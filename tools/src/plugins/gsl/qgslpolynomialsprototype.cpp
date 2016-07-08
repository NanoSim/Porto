#include <QtScript>
#include <QtCore>
#include <gsl/gsl_poly.h>
#include "qgslpolynomials.h"
#include "qgslpolynomialsprototype.h"

using soft::gsl::QGSLPolynomials;

QGSLPolynomialsPrototype :: QGSLPolynomialsPrototype (QObject *parent)
   : QObject (parent)
{}

QGSLPolynomialsPrototype :: ~QGSLPolynomialsPrototype()
{}

QScriptValue QGSLPolynomialsPrototype :: solveQuadratic(double a, double b, double c, QScriptValue callback)
{
   auto p = qscriptvalue_cast<QGSLPolynomials*>(thisObject());
   if (!p) {
      context()->throwError (QString (tr ("Cannot instanciate QGSLLinearRegression")));
   }

   double x0, x1;
   auto retval = gsl_poly_solve_quadratic(a, b, c, &x0, &x1);

   if (!callback.isNull() && callback.isFunction()) {
      auto act = context()->activationObject();
      auto args = QScriptValueList() << x0 << x1;
      callback.call(act, args);
   }
   return engine()->toScriptValue(retval);


}
				 
