#include <QScriptEngine>
#include <QScriptContext>
#include <QUdpSocket>

Q_DECLARE_METATYPE(QUdpSocket*)

#include "udpsocket.h"
#include "udpsocketprototype.h"

class UdpSocket::Private
{
  friend class UdpSocket;
  Private() 
    : proto (nullptr)
  {}

  UdpSocketPrototype *proto;
};

static QScriptValue createUdpSocket (QScriptContext * context, QScriptEngine *engine)
{
  auto parent = context->argument(0).toQObject();
  auto udpSocket = new QUdpSocket (parent);
  return engine->newQObject (udpSocket, QScriptEngine::ScriptOwnership);
}

UdpSocket :: UdpSocket (QObject *parent)
  : QObject (parent)
{}

UdpSocket :: UdpSocket (QScriptEngine * engine)
  : QObject (engine)
  , d (new UdpSocket::Private())
{
  Q_CHECK_PTR (engine);

  d->proto = new UdpSocketPrototype (engine);
  engine->setDefaultPrototype (qMetaTypeId <QUdpSocket*> (),
			       engine->newQObject (d->proto));

  auto ctor = engine->newFunction (createUdpSocket);
  auto metaObj = engine->newQMetaObject (&QUdpSocket::staticMetaObject, ctor);
  engine->globalObject().setProperty ("UdpSocket", metaObj);
}

UdpSocket :: ~UdpSocket ()
{}
