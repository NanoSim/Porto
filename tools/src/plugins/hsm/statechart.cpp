#include <QString>
#include <QList>
#include <QJsonObject>
#include <QJsonArray>
#include <QState>
#include <QFinalState>
#include "statechart.h"

Transition        buildTransition     (QJsonObject const &transitionObj);
QList<Transition> buildTransitionList (QJsonArray const &array);
State             buildState          (QJsonObject const &stateObj);
QList<State>      buildStateList      (QJsonArray const &array);
QAbstractState *  createState         (State const &s);

Transition buildTransition(QJsonObject const &transitionObj)
{
  Transition transition;
  transition.name = transitionObj.value("name").toString();
  transition.target = transitionObj.value("target").toString();
  return transition;
}

QList<Transition> buildTransitionList(QJsonArray const &array)
{
  QList<Transition> transitionList;
  auto it = array.constBegin();
  while (it != array.constEnd()) {
    transitionList.append (buildTransition((*it).toObject()));
    ++it;
  }
  return transitionList;
}

State buildState (QJsonObject const &stateObj)
{
  State state;

  state.name = stateObj.value("name").toString();
  auto transitionsValue = stateObj.value("transitions");
  if (!transitionsValue.isUndefined()) {
    state.transitions = buildTransitionList(transitionsValue.toArray());
  }
  auto typeValue = stateObj.value("type");
  if (!typeValue.isUndefined()) {
    state.type = typeValue.toString();
  }

  auto commandValue = stateObj.value("command");
  if (!commandValue.isUndefined()) {
    state.command = commandValue.toString();
  }
  
  return state;
}

QList<State> buildStateList(QJsonArray const &array)
{
  QList<State> stateList;
  auto it = array.constBegin();
  while (it != array.constEnd()) {
    stateList.append (buildState((*it).toObject()));
    ++it;
  }
  return stateList;
}

StateChart buildStateChart(QJsonObject const &obj)
{
  StateChart sc;
  sc.name         = obj.value("name").toString();
  sc.version      = obj.value("version").toString();
  sc.initialState = obj.value("initialState").toString();
  sc.states       = buildStateList(obj.value("states").toArray());
  return sc;
}

QAbstractState *createState(State const &s, QState *parent)
{
  if (s.type == "final")
    return new QFinalState();
  else
    return new QState(parent);
}
