#include <QtScript>
#include <QVariantList>
#include <QTextStream>
#include "qgsllinearregression.h"
#include "qgsllinearregressionprototype.h"

using porto::gsl::QGSLLinearRegression;

QGSLLinearRegressionPrototype :: QGSLLinearRegressionPrototype (QObject *parent)
   : QObject (parent)
{}

QGSLLinearRegressionPrototype :: ~QGSLLinearRegressionPrototype()
{}

QScriptValue QGSLLinearRegressionPrototype :: linear(QVariantList const &xs, QVariantList const &ys, QScriptValue callback)
{
   auto l = qscriptvalue_cast<QGSLLinearRegression*>(thisObject());
   if (!l) {
      context()->throwError (QString (tr ("Cannot instanciate QGSLLinearRegression")));
   }

   double c0, c1, cov00, cov01, cov11, sumsq;
   l->linear(xs, ys, c0, c1, cov00, cov01, cov11, sumsq);

   auto act = context()->activationObject();
   act.setProperty("c0", c0);
   act.setProperty("c1", c1);
   if (!callback.isNull() && callback.isFunction()) {
      callback.call(context()->activationObject(), QScriptValueList() << c0 << c1 << cov00 << cov01 << cov11 << sumsq);
   }

   return engine()->evaluate("(function (x){return (c0+c1*x);})");
}

QScriptValue QGSLLinearRegressionPrototype :: wlinear(QVariantList const &xs, QVariantList const &ws, QVariantList const &ys, QScriptValue callback)
{
   auto l = qscriptvalue_cast<QGSLLinearRegression*>(thisObject());
   if (!l) {
      context()->throwError (QString (tr ("Cannot instanciate QGSLLinearRegression")));
   }

   double c0, c1, cov00, cov01, cov11, chisq;
   l->wlinear(xs, ws, ys, c0, c1, cov00, cov01, cov11, chisq);
   auto act = context()->activationObject();
   act.setProperty("c0", c0);
   act.setProperty("c1", c1);

   if (!callback.isNull() && callback.isFunction()) {
      auto args = QScriptValueList() << c0 << c1 << cov00 << cov01 << cov11 << chisq;
      callback.call(act, args);
   }

   return engine()->evaluate("(function (x){return (c0+c1*x);})");
}

QScriptValue QGSLLinearRegressionPrototype :: linearEst(double x, double c0, double c1, double cov00, double cov01, double cov11, QScriptValue callback)
{
   auto l = qscriptvalue_cast<QGSLLinearRegression*>(thisObject());
   if (!l) {
      context()->throwError (QString (tr ("Cannot instanciate QGSLLinearRegression")));
   }

   double y, yErr;
   auto retval = l->linearEst(x, c0, c1, cov00, cov01, cov11, y, yErr);
   if (!callback.isNull() && callback.isFunction()) {
      auto act = context()->activationObject();
      auto args = QScriptValueList() << y << yErr;
      callback.call(act, args);
   }
   return engine()->toScriptValue(retval);
}

QScriptValue QGSLLinearRegressionPrototype :: mul (QVariantList const &xs, QVariantList const &ys, QScriptValue callback)
{
   auto l = qscriptvalue_cast<QGSLLinearRegression*>(thisObject());
   if (!l) {
      context()->throwError (QString (tr ("Cannot instanciate QGSLLinearRegression")));
   }
   double c1, cov11, sumsq;
   auto retval = l->mul(xs, ys, c1, cov11, sumsq);
   Q_UNUSED(retval);
   auto act = context()->activationObject();
   act.setProperty("c1", c1);
   if (!callback.isNull() && callback.isFunction()) {
      auto args = QScriptValueList() << c1 << cov11 << sumsq;
      callback.call(act, args);
   }

   return engine()->evaluate("(function(x){return(c1*x);})");
}

QScriptValue QGSLLinearRegressionPrototype :: wmul (QVariantList const &xs, QVariantList const &ws, QVariantList const &ys, QScriptValue callback)
{
   auto l = qscriptvalue_cast<QGSLLinearRegression*>(thisObject());
   if (!l) {
      context()->throwError (QString (tr ("Cannot instanciate QGSLLinearRegression")));
   }
   double c1, cov11, sumsq;
   auto retval = l->wmul(xs, ws, ys, c1, cov11, sumsq);
   Q_UNUSED(retval);
   auto act = context()->activationObject();
   act.setProperty ("c1", c1);
   if (!callback.isNull() && callback.isFunction()) {
      auto args = QScriptValueList() << c1 << cov11 << sumsq;
      callback.call (act, args);
   }

   return engine()->evaluate("(function(x){return(c1*x);})");
}
