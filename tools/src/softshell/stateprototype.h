#ifndef STATEPROTOTYPE_H_DEF
#define STATEPROTOTYPE_H_DEF

#include <QScriptable>
#include <QScriptValue>
#include <QObject>
#include <QString>

class StatePrototype : public QObject
		     , public QScriptable
{
  Q_OBJECT
public:
  explicit StatePrototype (QObject *parent = nullptr);
  virtual ~StatePrototype();

public slots:
  QScriptValue addTransition(QScriptValue &obj, QString const &signal, QScriptValue &target);
};

#endif // STATEPROTOTYPE_H_DEF
  
