#include <QTextStream>
#include <QState>
#include "protocall.h"
#include "stateprototype.h"
#include "state.h"

StatePrototype :: StatePrototype (QObject *parent)
  : QObject (parent)
{}

StatePrototype :: ~StatePrototype()
{}

QScriptValue StatePrototype :: addTransition(QScriptValue &obj, QString const &signal, QScriptValue &target)
{
  
  return protocall_void <QState> (thisObject(), this->parent(),
			     [=](QState *state)
    {
      auto aState = target.toQObject();
      state->addTransition(obj.toQObject(), qPrintable(signal), qobject_cast<QAbstractState*>(aState));
    });
  
  return QScriptValue();
}
