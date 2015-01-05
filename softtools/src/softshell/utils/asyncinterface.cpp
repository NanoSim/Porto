#include <QScriptEngine>
#include <QScriptContext>
#include <QScriptValue>
#include "async.h"
#include "asyncprototype.h"
#include "asyncinterface.h"

Q_DECLARE_METATYPE (Async*)

class AsyncInterface::Private
{
  friend class AsyncInterface;
  Private(QScriptEngine *engine)
    : proto(new AsyncPrototype (engine))
  {}

  AsyncPrototype *proto;  
};

static QScriptValue createAsync (QScriptContext *ctx, QScriptEngine *engine)
{
  Q_UNUSED(ctx);
  auto async = new Async(engine);
  return engine->newQObject (async, QScriptEngine::AutoOwnership);
}

AsyncInterface :: AsyncInterface (QObject *parent)
  : QObject (parent)
{}

AsyncInterface :: AsyncInterface (QScriptEngine *engine)
  : QObject (engine)
  , d (new AsyncInterface::Private(engine))    
{
  engine->setDefaultPrototype (qMetaTypeId <Async*>(),
			       engine->newQObject (d->proto));
  auto ctor = engine->newFunction (createAsync);
  auto metaObj = engine->newQMetaObject (&Async::staticMetaObject, ctor);
  engine->globalObject().setProperty ("Async", metaObj);
}

AsyncInterface :: ~AsyncInterface ()
{}
