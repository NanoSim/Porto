#include <QScriptEngine>
#include <QScriptContext>
#include <QCoreApplication>
#include <QEventLoop>

#include "application.h"
#include "applicationprototype.h"

Q_DECLARE_METATYPE(QCoreApplication*)

class Application::Private
{
   friend class Application;
   Private () {}

  ApplicationPrototype *proto;
};

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
   globalObject.setProperty("Application", engine->newQObject(qApp));
}

Application :: ~Application()
{}
