#include <QtCore>
#include <QtConcurrent>
#include <QtScript>
#include <functional>
#include "statechart.h"
#include "scenario.h"
#include "workflow.h"
#include "prochandle.h"

Q_DECLARE_METATYPE(QFutureWatcher<QScriptValue>*)

namespace {

  static QJsonDocument toJson(QScriptEngine *engine, QScriptValue const &obj)
  {
    QScriptContext *ctx = engine->pushContext();
    ctx->activationObject().setProperty("obj", obj);
    QScriptValue ret = engine->evaluate("JSON.stringify(obj)");
    QJsonParseError error;
    QJsonDocument retval = QJsonDocument::fromJson(ret.toString().toLocal8Bit(), &error);
    
    if (error.error != QJsonParseError::NoError) {
      qWarning() << "Parse error: " << error.errorString();
    }
    
    return retval;
  }

  QJsonObject jsonObjectFromScriptValue(QScriptEngine *engine, QScriptValue const &value)
  {
    auto doc = toJson(engine, value);
    return doc.object();
  }

  static QScriptValue runStateMachine(QString const &arg, StateChart const &stateChart, QProcessEnvironment const &processEnvironment)
  {
    QEventLoop eventLoop;
    QStateMachine machine;
    QMap<QString, QAbstractState*> stateMap;
    auto process = new QProcess(&eventLoop);
    ProcHandle *procHandle = new ProcHandle(process, &eventLoop);
    
    process->setProcessEnvironment(processEnvironment);
    
    foreach (auto s, stateChart.states) {
      auto abstractState = createState(s);
      stateMap.insert(s.name, abstractState);
      machine.addState(abstractState);
      if (s.type.isEmpty()) {	
	auto state = qobject_cast<QState*>(abstractState);

	QObject::connect(state, &QState::entered, [=](){
	    process->start(s.command, QStringList() << arg);
	  });
      }
    }

    for(auto s: stateChart.states) {
      auto state = qobject_cast<QState*>(stateMap.value(s.name));
      foreach (auto t, s.transitions) {
	if (0 == QString::compare(t.name, "normalExit", Qt::CaseInsensitive)) {
	  state->addTransition(procHandle, SIGNAL(normalExit()), stateMap[t.target]);
	}      
	else if (0 == QString::compare(t.name, "exitFailure", Qt::CaseInsensitive)) {
	  state->addTransition(procHandle, SIGNAL(exitFailure()), stateMap[t.target]);
	}
      }
    }

    machine.setInitialState (stateMap.value(stateChart.initialState));
    QObject::connect (&machine, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    machine.start();

    return eventLoop.exec();    
  }

  static QProcessEnvironment buildProcessEnvironment(QScriptEngine *engine)
  {
    auto envScriptValue = engine->evaluate ("(function(){return(JSON.stringify(Ats.Workflow.environment));})()");
    auto jsonDocument   = QJsonDocument::fromJson (envScriptValue.toString().toLocal8Bit());
    QProcessEnvironment processEnvironment(QProcessEnvironment::systemEnvironment());
    for (auto key : jsonDocument.object().keys()) {
      processEnvironment.insert(key, jsonDocument.object().value(key).toString());
    }
    return processEnvironment;
  }

  /*!
    runWorkflow
   */
  static QScriptValue runWorkflow (QScriptContext *ctx, QScriptEngine *engine)
  {
    using namespace std::placeholders;
    
    if (ctx->argumentCount() < 2
	|| !ctx->argument(0).isObject()
	|| !ctx->argument(1).isObject()) {
      ctx->throwError("Syntax error");
      return engine->undefinedValue();
    }

    if (ctx->argumentCount() >= 3 && !ctx->argument(2).isFunction()) {
      ctx->throwError("Syntax error");
      return engine->undefinedValue();
    }

    auto stateChart         = buildStateChart (jsonObjectFromScriptValue (engine, ctx->argument (0)));
    auto scenario           = buildScenario   (jsonObjectFromScriptValue (engine, ctx->argument (1)));
    auto callback           = (ctx->argumentCount() >= 3 ? ctx->argument(2) : engine->undefinedValue());
    auto processEnvironment = buildProcessEnvironment(engine);    
    auto fn                 = bind(&runStateMachine, _1, stateChart, processEnvironment);

    if (ctx->argumentCount() < 3) {
      QtConcurrent::blockingMapped(scenario.refs, fn);
    } else {
      QFuture<QScriptValue> future = QtConcurrent::mapped(scenario.refs, fn);
      auto futureWatcher = new QFutureWatcher<QScriptValue>();
      futureWatcher->setFuture (future);
      auto watcher  = engine->newQObject (futureWatcher, QScriptEngine::ScriptOwnership);
      auto callback = ctx->argument(2);
      auto res      = callback.call (callback, QScriptValueList() << watcher);
    }

    return engine->undefinedValue();
  }

  static QScriptValue setProcessEnv (QScriptContext *ctx, QScriptEngine *engine)
  {
    auto scriptable = ctx->thisObject().property("environment");
    auto jsonDoc = QJsonDocument::fromJson(engine->evaluate("(function(){return(JSON.stringify(Ats.Workflow.environment));})()").toString().toLocal8Bit());
    QProcessEnvironment processEnv;
    for (auto key : jsonDoc.object().keys()) {
      processEnv.insert (key, jsonDoc.object().value(key).toString());
    }
    return engine->undefinedValue();
  }
} // namespace

template <class Function>
static void registerFunction (QScriptEngine *engine, 
			      QString const & name, 
			      Function fn, 
			      QScriptValue *obj,
			      QString const & doc = QString())
{
  auto scriptFunction = engine->newFunction(fn);
  if (!doc.isEmpty()) {
    scriptFunction.setProperty("^doc", doc);
  }
  obj->setProperty(name, scriptFunction);
}

Workflow :: ~Workflow()
{}


void Workflow :: registerPlugin (QScriptEngine *engine)
{
  Q_CHECK_PTR (engine);
  auto globalObj   = engine->globalObject();
  auto ats         = engine->newObject();
  auto wf          = engine->newObject();
  auto environment = engine->newObject();
  globalObj.setProperty("Ats", ats);
  ats.setProperty("Workflow", wf);
  wf.setProperty("environment", environment);

  registerFunction (engine, "run", &::runWorkflow, &wf, "Run the current workflow");
  registerFunction (engine, "setProcessEnvironment", &::setProcessEnv, &wf,
    "Sets the environment that the workflow will use when starting a process to environment object");
}
