#ifndef STATECHART_H_DEF
#define STATECHART_H_DEF

class QAbstractState;
struct Transition
{
  QString target;
  QString name;
};

struct State
{
  QString name;
  QString type;
  QString command;
  QList<Transition> transitions;
};

struct StateChart
{
  QString name;
  QString version;
  QString initialState;
  QList<State> states;
};

StateChart        buildStateChart     (QJsonObject const &obj);
QAbstractState *  createState         (State const &s, QState *parent = 0);  // TFH: Doesn't belong here (?)
#endif // STATECHART_H_DEF
