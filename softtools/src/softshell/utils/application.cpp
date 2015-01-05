#include <QScriptEngine>
#include <QScriptContext>
#include <QCoreApplication>
#include <QThreadPool>
#include <QEventLoop>

#include "application.h"
#include "applicationprototype.h"
#include "registerfunction.h"

Q_DECLARE_METATYPE(QCoreApplication*)

class Application::Private
{
   friend class Application;
   Private () {}

  ApplicationPrototype *proto;
};

namespace {
  static QScriptValue maxThreadCount(QScriptContext *context, QScriptEngine *engine)
  {
    Q_UNUSED(context);
    return engine->toScriptValue(QThreadPool::globalInstance()->maxThreadCount());
  }

  static QScriptValue setMaxThreadCount(QScriptContext *context, QScriptEngine *engine)
  {
    if (context->argumentCount() < 1) {
      context->throwError("Missing arguments");
      return engine->undefinedValue();
    }
    if (!context->argument(0).isNumber()) {
      context->throwError("Syntax Error: The argument should be a number");
      return engine->undefinedValue();
    }
    auto numThreads = context->argument(0).toInteger();
    QThreadPool::globalInstance()->setMaxThreadCount(numThreads);
    return engine->toScriptValue(numThreads == QThreadPool::globalInstance()->maxThreadCount());
  }
}

Application :: Application (QObject *parent)
   : QObject (parent)
{}

Application :: Application (QScriptEngine *engine)
   : QObject (engine)
   , d (new ::Application::Private())
{
   Q_CHECK_PTR (engine);
   
   d->proto = new ApplicationPrototype(engine);
   engine->setDefaultPrototype (qMetaTypeId<QCoreApplication*>(),
				engine->newQObject(d->proto));

   auto globalObject = engine->globalObject();
   auto application = engine->newQObject(qApp);
   globalObject.setProperty("Application", application);
   registerFunction (engine, "maxThreadCount", ::maxThreadCount, &application, "Returns the maximum number of threads used by the thread pool");
   registerFunction (engine, "setMaxThreadCount", ::setMaxThreadCount, &application, "Set the maximum number of threads used by the thread pool");  
}

Application :: ~Application()
{}
