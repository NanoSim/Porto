#ifndef QGSLLINEARREGRESSIONPROTOTYPE_H_DEF
#define QGSLLINEARREGRESSIONPROTOTYPE_H_DEF

#include <QObject>
#include <QScriptable>
#include <QScriptValue>

class QGSLLinearRegressionPrototype : public QObject
				    , public QScriptable
{
   Q_OBJECT
public:
   explicit QGSLLinearRegressionPrototype (QObject *parent = nullptr);
   virtual ~QGSLLinearRegressionPrototype();

public slots:
   QScriptValue linear(QVariantList const &xs, QVariantList const &ys, QScriptValue callback = QScriptValue());
   QScriptValue wlinear(QVariantList const &xs, QVariantList const &ws, QVariantList const &ys, QScriptValue callback = QScriptValue());
   QScriptValue linearEst(double x, double c0, double c1, double cov00, double cov01, double cov011, QScriptValue callback);
   QScriptValue mul (QVariantList const &xs, QVariantList const &ys, QScriptValue callback);
   QScriptValue wmul (QVariantList const &xs, QVariantList const &ws, QVariantList const &ys, QScriptValue callback);

};

#endif // QGSLLINEARREGRESSIONPROTOTYPE_H_DEF
