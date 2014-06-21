#include <QScriptEngine>
#include <QScriptContext>
#include <QString>
#include "httpserver.h"

#include "httpd.h"
#include "httpdprototype.h"

Q_DECLARE_METATYPE(HttpServer)

class Httpd::Private
{
  friend class Httpd;
  Private() : proto (nullptr) {}

  HttpdPrototype *proto;
};

static QScriptValue createHttpd(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() == 0) {
    context->throwError (QString ("You need to specify a port number"));
    return engine->toScriptValue(false);
  }

  if (context->argumentCount() > 0 && !context->argument(0).isNumber()) {
    context->throwError (QString ("Illegal port number"));
    return engine->toScriptValue(false);
  }
  
  auto port = context->argument(0).toUInt16();
  auto httpd = new HttpServer (port, engine);
  return engine->newQObject (httpd, QScriptEngine::ScriptOwnership);
}

Httpd :: Httpd (QObject *parent)
  : QObject (parent)
{}

Httpd :: Httpd (QScriptEngine *engine)
  : QObject (engine)
  , d (new Httpd::Private())
{
  Q_CHECK_PTR (engine);
  d->proto = q_check_ptr (new HttpdPrototype (engine));
  engine->setDefaultPrototype (qMetaTypeId<HttpServer*>(),
			       engine->newQObject (d->proto));
  auto ctor = engine->newFunction (createHttpd);
  auto metaObj = engine->newQMetaObject (&HttpServer::staticMetaObject, ctor);
  auto global  = engine->globalObject();
  global.setProperty ("HttpServer", metaObj);
}

Httpd :: ~Httpd ()
{}

