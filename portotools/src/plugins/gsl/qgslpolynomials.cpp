#include "qgslpolynomials.h"
#include <gsl/gsl_poly.h>

PORTO_BEGIN_NAMESPACE
GSL_BEGIN_NAMESPACE

QGSLPolynomials :: QGSLPolynomials (QObject *parent)
   : QObject (parent)
{}

QGSLPolynomials ::  ~QGSLPolynomials()
{}

int QGSLPolynomials :: solveQuadratic (double a, double b, double c, double &x0, double &x1)
{
   return gsl_poly_solve_quadratic(a, b, c, &x0, &x1);
}

GSL_END_NAMESPACE
PORTO_END_NAMESPACE
