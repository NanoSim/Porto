#include <QSsl>
#include <QScriptEngine>
#include <QScriptContext>
#include <QNetworkAccessManager>
#include "networkaccessmanager.h"
#include "networkaccessmanagerprototype.h"

Q_DECLARE_METATYPE (QNetworkAccessManager*)

class NetworkAccessManager :: Private
{
  friend class NetworkAccessManager;
  Private()
  {}

  NetworkAccessManagerPrototype *proto;
};

static QScriptValue createNetworkAccessManager (QScriptContext *context,
						QScriptEngine *engine)
{
  QNetworkAccessManager *nam = new QNetworkAccessManager();
  Q_CHECK_PTR (nam);
  return engine->newQObject (nam, QScriptEngine::ScriptOwnership);
}

NetworkAccessManager :: NetworkAccessManager (QObject *parent)
  : QObject (parent)
{}

NetworkAccessManager :: NetworkAccessManager (QScriptEngine *engine)
  : QObject (engine)
  , d (new NetworkAccessManager::Private())
{
  Q_CHECK_PTR(engine);
  Q_CHECK_PTR(d);
 
  d->proto = q_check_ptr (new NetworkAccessManagerPrototype (engine));
  engine->setDefaultPrototype (qMetaTypeId<QNetworkAccessManager*>(),
			       engine->newQObject (d->proto));
  auto ctor    = engine->newFunction (createNetworkAccessManager);
  auto metaObj = engine->newQMetaObject (&QNetworkAccessManager::staticMetaObject, ctor);
  auto global  = engine->globalObject();
  global.setProperty ("NetworkAccessManager", metaObj);
}

NetworkAccessManager :: ~NetworkAccessManager()
{
  delete d->proto;
}
