#include <QObject>
#include <QScriptEngine>
#include <QCoreApplication>
#include "applicationprototype.h"

ApplicationPrototype :: ApplicationPrototype (QObject * parent)
  : QObject(parent)
{}

ApplicationPrototype :: ~ApplicationPrototype ()
{}

QScriptValue ApplicationPrototype :: exec()
{
  auto engine = qobject_cast<QScriptEngine*>(this->parent());
  return engine->toScriptValue(qApp->exec());
}
