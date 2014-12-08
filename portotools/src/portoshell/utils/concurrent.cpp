#include <QScriptEngine>
#include <QScriptContext>
#include <QScriptValue>
#include <QScriptValueIterator>
#include <QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>
#include <QFutureSynchronizer>

#include "concurrent.h"
#include "registerfunction.h"

Q_DECLARE_METATYPE(QFutureWatcher<QScriptValue>*)


class Concurrent::Private
{
  friend class Concurrent;
  Private() {};

  //  QFutureSynchronizer<QScriptValue> futureSynchronizer;
};

namespace {
  static QScriptValue concurrent_exec(QString const &path, QStringList const &args) {
    return QScriptValue (QProcess::execute(path, args));
  }
  
  static QScriptValue execute (QScriptContext *context, QScriptEngine *engine)
  {
    QString path;
    QStringList args;
    if (context->argumentCount() < 1) {
      context->throwError("Missing arguments");
      return engine->undefinedValue();
      
    } else if (context->argumentCount() == 1) {
      path = context->argument(0).toString();
	
    } else if (context->argumentCount() > 1) {
      path = context->argument(0).toString();
      auto arguments = context->argument(1);
      if (!arguments.isArray()) {
	context->throwError("Syntax Error");
	return engine->undefinedValue();
      }
      qScriptValueToSequence(arguments, args);
    }

    QFutureWatcher<QScriptValue> *watcher = new QFutureWatcher<QScriptValue>();
    watcher->setFuture(QtConcurrent::run(concurrent_exec, path, args));
    return engine->newQObject(watcher, QScriptEngine::ScriptOwnership, 0);
  }
  
  
} // anon namespace

Concurrent :: Concurrent (QObject *parent)
  : QObject (parent)
{}

Concurrent :: Concurrent (QScriptEngine *engine)
  : QObject (engine)
  , d (new ::Concurrent::Private())
{
  Q_CHECK_PTR(engine);
  auto globalObj = engine->globalObject();
  auto concurrent = engine->newObject();
  globalObj.setProperty("Concurrent", concurrent);

  registerFunction (engine, "execute", ::execute, &concurrent, "Starts the program program with the arguments arguments in a new process");
}

Concurrent :: ~Concurrent()
{}
