#include <QHostInfo>
#include <QScriptContext>
#include <QScriptEngine>
#include <QScriptValue>
#include "hostinfo.h"

namespace {
  static QScriptValue localDomainName(QScriptContext *, QScriptEngine *engine) {
    return engine->toScriptValue(QHostInfo::localDomainName());
  }

  static QScriptValue localHostName(QScriptContext *, QScriptEngine *engine) {
    return engine->toScriptValue(QHostInfo::localHostName());
  }
}

template <class Function>
static void registerFunction (QScriptEngine *engine, 
			      QString const & name, 
			      Function fn, 
			      QScriptValue *obj,
			      QString const & doc = QString())
{
   auto scriptFunction = engine->newFunction(fn);
   if (!doc.isEmpty()) {
      scriptFunction.setProperty("^doc", doc);
   }
   obj->setProperty(name, scriptFunction);
}

HostInfo ::HostInfo (QScriptEngine *engine)
  : QObject (engine)
{
  Q_CHECK_PTR (engine);
  auto globalObj = engine->globalObject();
  auto hostInfo = engine->newObject();
  globalObj.setProperty("hostInfo", hostInfo);

  registerFunction (engine, "localDomainName", &::localDomainName, &hostInfo, "Returns the DNS domain of this machine.");
  registerFunction (engine, "localHostName", &::localHostName, &hostInfo, "Returns the host name of this machine.");
}

HostInfo ::  ~HostInfo()
{}

