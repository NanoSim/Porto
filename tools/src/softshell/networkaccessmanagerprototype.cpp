#include <QScriptEngine>
#include <QNetworkReply>
#include <QNetworkRequest>
#include "networkaccessmanagerprototype.h"

#include "protocall.h"


NetworkAccessManagerPrototype :: NetworkAccessManagerPrototype(QObject *parent)
  : QObject (parent)
{
    auto nam = qscriptvalue_cast<QNetworkAccessManager*> (thisObject());
    if (nam) {
      connect (nam, SIGNAL(finished(QNetworkReply*)), SIGNAL(finished(QNetworkReply*)));
      connect (nam, SIGNAL(encrypted(QNetworkReply*)), SIGNAL(encrypted(QNetworkReply*)));
    }
}

NetworkAccessManagerPrototype ::  ~NetworkAccessManagerPrototype()
{}

QScriptValue NetworkAccessManagerPrototype :: get(QString const &url)
{
  return protocall <QNetworkAccessManager>(thisObject(), this->parent(),
					   [=](QNetworkAccessManager *mgr)
					   -> QNetworkReply*
    {
      return mgr->get(QNetworkRequest(QUrl(url)));
    });
}
