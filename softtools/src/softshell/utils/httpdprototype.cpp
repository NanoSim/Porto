#include <QtCore>
#include <QScriptEngine>
#include "httpserver.h"
#include "httpdprototype.h"
#include "protocall.h"

HttpdPrototype :: HttpdPrototype (QObject *parent)
  : QObject (parent)
{
  auto httpd = qscriptvalue_cast<HttpServer*> (thisObject());
  if (httpd) {
    // TODO: Connect httpserver signals and slots
  }
}

HttpdPrototype :: ~HttpdPrototype()
{}

QScriptValue HttpdPrototype :: start()
{
  return protocall <HttpServer> (thisObject(), this->parent(),
				 [](HttpServer* httpd)
				 -> bool
    {
      return httpd->start();
    });
}

QScriptValue HttpdPrototype :: stop()
{
  return protocall <HttpServer> (thisObject(), this->parent(),
				 [](HttpServer* httpd)
				 -> bool
    {
      return httpd->stop();
    });
}

QScriptValue HttpdPrototype :: setRootDir(QString const &path)
{
  return protocall <HttpServer> (thisObject(), this->parent(),
				 [=](HttpServer *httpd)
				 -> bool
    {
      return httpd->setRootDir(path);
    });
}

