#include <QScriptEngine>
#include <QScriptContext>
#include <QNetworkReply>
#include "networkreply.h"
#include "networkreplyprototype.h"

Q_DECLARE_METATYPE (QNetworkReply*)

class NetworkReply :: Private
{
  friend class NetworkReply;
  Private()
  {}
  
  NetworkReplyPrototype *proto;
};

/*
static QScriptValue createNetworkReply (QScriptContext *context,
					QScriptEngine *engine)
{
  QNetworkReply *networkReply = new QNetworkReply();
  Q_CHECK_PTR (networkReply);
  return engine->newQObject (networkReply, QScriptEngine::ScriptOwnership);
}
*/

NetworkReply :: NetworkReply (QObject *parent)
  : QObject (parent)
{}

NetworkReply :: NetworkReply (QScriptEngine *engine)
  : QObject (engine)
  , d (new NetworkReply::Private())
{
  Q_CHECK_PTR (engine);
  Q_CHECK_PTR (d);

  d->proto = q_check_ptr (new NetworkReplyPrototype (engine));
  engine->setDefaultPrototype( qMetaTypeId<QNetworkReply*>(),
			       engine->newQObject (d->proto));
  //  auto ctor    = engine->newFunction (createNetworkReply);
  auto metaObj = engine->newQMetaObject (&NetworkReply::staticMetaObject);
  auto global  = engine->globalObject();
  global.setProperty ("NetworkReply", metaObj);
}

NetworkReply :: ~NetworkReply()
{
  delete d->proto;
}

