#include <QCoreApplication>
#include <QScriptEngine>

#include "automodule.h"

QScriptEngine *ScriptEngine::instance = nullptr;

QScriptEngine *ScriptEngine :: globalEngine()
{
  if (!instance)
    instance = new QScriptEngine(qApp);

  return instance;
}
