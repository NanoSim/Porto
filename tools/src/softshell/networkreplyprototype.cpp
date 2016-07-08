#include <QScriptEngine>
#include <QNetworkReply>
#include "networkreplyprototype.h"
#include "networkreply.h"
#include "protocall.h"

NetworkReplyPrototype :: NetworkReplyPrototype(QObject *parent)
  : QObject (parent)
{}

NetworkReplyPrototype :: ~NetworkReplyPrototype()
{}

QScriptValue NetworkReplyPrototype :: readAll()
{
  return protocall <QNetworkReply>(thisObject(), this->parent(),
				   [](QNetworkReply *reply)
				   -> QByteArray
    {
      return reply->readAll();
    });
}

QScriptValue NetworkReplyPrototype :: readBufferSize()
{
  return protocall <QNetworkReply>(thisObject(), this->parent(),
				   [](QNetworkReply *reply)
				   -> qint64
    {
      return reply->readBufferSize();
    });
}

QScriptValue NetworkReplyPrototype :: waitForReadyRead(int msecs)
{
    return protocall <QNetworkReply>(thisObject(), this->parent(),
				     [=](QNetworkReply *reply)
				     -> bool
      {
	return reply->waitForReadyRead(msecs);
      });
}
