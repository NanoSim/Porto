#ifndef QNLOPTPROTOTYPE_H_DEF
#define QNLOPTPROTOTYPE_H_DEF

#include <QObject>
#include <QScriptable>
#include <QScriptValue>

class QNLoptPrototype : public QObject,
			public QScriptable
{
  Q_OBJECT
public:
  explicit QNLoptPrototype (QObject *parent = nullptr);
  virtual ~QNLoptPrototype();

public slots:
  QScriptValue setMinObjective (QScriptValue callback, QScriptValue fData = QScriptValue());
  QScriptValue addInequalityConstraint (QScriptValue callback, QScriptValue constraintData, QScriptValue tolerance = QScriptValue());
  QScriptValue testCallback(QScriptValue callback = QScriptValue());
};

#endif // QNLOPTPROTOTYPE_H_DEF
