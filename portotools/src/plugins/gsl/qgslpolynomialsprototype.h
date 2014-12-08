#ifndef QGSLPOLYNOMIALSPROTOTYPE_H_DEF
#define QGSLPOLYNOMIALSPROTOTYPE_H_DEF

#include <QObject>
#include <QScriptable>
#include <QScriptValue>

class QGSLPolynomialsPrototype : public QObject
			       , public QScriptable
{
   Q_OBJECT

public:
   explicit QGSLPolynomialsPrototype (QObject *parent = nullptr);
   virtual ~QGSLPolynomialsPrototype();

public slots:
   QScriptValue solveQuadratic(double a, double b, double c, QScriptValue callback);
};
				 

#endif // QGSLPOLYNOMIALSPROTOTYPE_H_DEF
