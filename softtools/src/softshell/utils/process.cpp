#include <QScriptEngine>
#include <QScriptContext>
#include <QCoreApplication>
#include <QDir>
#include <QProcess>
#include <QVector>
#include <QString>
#include <QTextStream>
#include "process.h"
#include "processprototype.h"

Q_DECLARE_METATYPE(QProcess*)
namespace process {
   static QScriptValue pwd (QScriptContext *, QScriptEngine *engine)
   {
      auto path = QDir::currentPath();
      return engine->toScriptValue(path);
   }

   static QScriptValue env (QScriptContext *context, QScriptEngine *engine)
   {
      auto environmentObj = engine->newObject();
      foreach (auto env, QProcess::systemEnvironment()) {
	 auto idx = env.indexOf("=");
	 if (idx < 0) {
	    context->throwError("QProcess::systemEnvironment failure");
	 }
	 auto tuple = qMakePair(env.left(idx), env.mid(idx+1));
	 environmentObj.setProperty(tuple.first, engine->toScriptValue(tuple.second));
      }
      return environmentObj;
   }

   static QScriptValue chdir (QScriptContext *context, QScriptEngine *engine)
   {
      if (context->argumentCount() < 1) return engine->toScriptValue(false);
      auto path = context->argument(0).toString();
      auto isOk = QDir::setCurrent (path);
      return engine->toScriptValue(isOk);
   }

   static QScriptValue setSearchPaths (QScriptContext *context, QScriptEngine *engine)
   {
      if (context->argumentCount() < 2 ||
	  !context->argument(0).isString() || 
	  !context->argument(1).isArray()) {
	 return engine->toScriptValue(false);
      }
      
      auto prefix = context->argument(0).toString();
      QStringList searchPaths;
      qScriptValueToSequence(context->argument(1), searchPaths);
      QDir::setSearchPaths(prefix, searchPaths);
      return engine->toScriptValue(true);
   }

   static QScriptValue searchPaths (QScriptContext *context, QScriptEngine *engine)
   {
      if (context->argumentCount() < 1 ||
	  !context->argument(0).isString()) {
	 return engine->toScriptValue(false);
      }
      
      auto prefix = context->argument(0).toString();
      auto searchPaths = QDir::searchPaths(prefix);
      return qScriptValueFromSequence(engine, searchPaths);
   }
      
}

static QScriptValue createProcess(QScriptContext *, QScriptEngine *engine)
{
   QProcess *process = new QProcess(engine);
   return engine->newQObject(process, QScriptEngine::ScriptOwnership);
}

class Process::Private
{
   friend class Process;
   Private() {}

   ProcessPrototype *proto;
};

Process :: Process (QObject * parent)
   : QObject (parent)
{}

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

Process :: Process(QScriptEngine *engine)
   : QObject (engine)
   , d (new Process::Private())
{
   Q_CHECK_PTR (engine);

   d->proto = new ProcessPrototype(engine);
   engine->setDefaultPrototype (qMetaTypeId <QProcess*> (),
				engine->newQObject (d->proto));

   auto ctor = engine->newFunction (createProcess);
   auto metaObj = engine->newQMetaObject (&QProcess::staticMetaObject, ctor);
   auto globalObj = engine->globalObject();
   globalObj.setProperty ("Process", metaObj);
   auto proc = engine->newObject();
   globalObj.setProperty ("process", proc);

   registerFunction (engine, "pwd",            process::pwd, &proc, "Prints the current working directory of the process");
   registerFunction (engine, "environment",    process::env, &proc, "An object containing the user environment");
   registerFunction (engine, "chdir",          process::chdir, &proc, "Change the current working directory for the process");
   registerFunction (engine, "setSearchPaths", process::setSearchPaths, &proc, "Sets or replaces search paths for file names with a named prefix");
   registerFunction (engine, "searchPaths",    process::searchPaths, &proc, "Returns the search paths for prefix.");
}

Process :: ~Process()
{}

