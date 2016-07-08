#ifndef STATE_H_DEF
#define STATE_H_DEF

#include <QObject>
#include <QScopedPointer>

class QScriptEngine;
class State : public QObject
{
  Q_OBJECT
public:
  explicit State (QScriptEngine *engine);
  virtual ~State();

private:
  class Private;
  QScopedPointer<Private> d;

  State (QObject *parent = nullptr);
};

#endif // STATE_H_DEF
