#ifndef QGSLSPECIALPROTOTYPE_H_DEF
#define QGSLSPECIALPROTOTYPE_H_DEF

#include <QObject>
#include <QScriptable>
#include <QScriptValue>

class QGSLSpecialPrototype : public QObject
			   , public QScriptable
{
   Q_OBJECT
public:
   explicit QGSLSpecialPrototype (QObject *parent = nullptr);
   virtual ~QGSLSpecialPrototype();

public slots:
   QScriptValue airyAi (double x, QScriptValue callback = QScriptValue());
   QScriptValue airyBi (double x, QScriptValue callback = QScriptValue());
   QScriptValue airyAiScaled (double x, QScriptValue callback = QScriptValue());
   QScriptValue airyBiScaled (double x, QScriptValue callback = QScriptValue());
};

#endif // QGSLSPECIALPROTOTYPE_H_DEF
