#include <QtScript/QScriptEngine>
#include <QtScript/QScriptValue>
#include <QtCore/QScopedPointer>
#include <QtCore/QString>
#include "scriptengine.h"

SOFT_BEGIN_NAMESPACE

class ScriptEngine::Private
{
   Private() 
      : engine(new QScriptEngine())
   {}

   ~Private()
   {}
   
   friend class ScriptEngine;
   QScopedPointer<QScriptEngine> engine;
};

ScriptEngine :: ScriptEngine()
   : d(new Private())
{}

ScriptEngine :: ~ScriptEngine()
{}

std::string ScriptEngine :: evaluate(std::string const &program,std::string const &fileName, int lineNumber)
{
   auto v = d->engine->evaluate(QString::fromStdString(program),
				QString::fromStdString(fileName),
				lineNumber);
   return v.toString().toStdString();
}

void *ScriptEngine :: ref() const
{
   return d->engine.data();
}


SOFT_END_NAMESPACE
