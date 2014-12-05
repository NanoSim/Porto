#include <QScriptEngine>
#include <QScriptContext>
#include <QEventLoop>

#include "eventloop.h"
#include "eventloopprototype.h"

Q_DECLARE_METATYPE(QEventLoop*)

class EventLoop::Private
{
  friend class EventLoop;
  Private() {}

  EventLoopPrototype *proto;
};

static QScriptValue createEventLoop(QScriptContext *, QScriptEngine *engine)
{
  QEventLoop *eventLoop = new QEventLoop (engine);
  Q_CHECK_PTR (eventLoop);
  return engine->newQObject(eventLoop, QScriptEngine::ScriptOwnership);
}

EventLoop :: EventLoop (QObject * parent)
   : QObject (parent)
{}

EventLoop :: EventLoop (QScriptEngine *engine)
   : QObject(engine)
   , d(new ::EventLoop::Private())
{
  Q_CHECK_PTR (engine);
  d->proto = q_check_ptr (new EventLoopPrototype (engine));
  engine->setDefaultPrototype (qMetaTypeId<QEventLoop*>(),
			       engine->newQObject (d->proto));
  
  auto ctor = engine->newFunction (createEventLoop);
  auto metaObj = engine->newQMetaObject (&QEventLoop::staticMetaObject, ctor);
  engine->globalObject().setProperty ("EventLoop", metaObj);
}

EventLoop :: ~EventLoop()
{}

