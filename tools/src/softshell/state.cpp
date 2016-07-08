#include <QScriptEngine>
#include <QScriptContext>
#include <QtCore>
#include <QState>

#include "state.h"
#include "stateprototype.h"

Q_DECLARE_METATYPE(QState*)

class State::Private
{
  friend class State;
  Private()
    : proto (nullptr)
  {}
  
  StatePrototype *proto;
};

static QScriptValue createState(QScriptContext *, QScriptEngine *engine)
{
  QState *state = new QState();
  Q_CHECK_PTR(state);
  return engine->newQObject(state, QScriptEngine::ScriptOwnership);
}

State :: State (QObject *parent)
  : QObject (parent)
{}

State :: State (QScriptEngine *engine)
  : QObject(engine)
  , d(new ::State::Private())
{
  Q_CHECK_PTR (engine);
  d->proto = q_check_ptr (new StatePrototype (engine));
  engine->setDefaultPrototype (qMetaTypeId<QState*>(),
			       engine->newQObject (d->proto));

  auto ctor = engine->newFunction (createState);
  auto metaObj = engine->newQMetaObject (&QState::staticMetaObject, ctor);
  auto globalObj = engine->globalObject();
  globalObj.setProperty("State", metaObj);
}

State :: ~State()
{}
