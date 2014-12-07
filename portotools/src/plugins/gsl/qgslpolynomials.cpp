#include "qgslpolynomials.h"
#include <gsl/gsl_poly.h>

PORTO_BEGIN_NAMESPACE
GSL_BEGIN_NAMESPACE

QGSLPolynomials :: QGSLPolynomials (QObject *parent)
   : QObject (parent)
{}

QGSLPolynomials ::  ~QGSLPolynomials()
{}

/**
   @brief This function finds the real roots of the quadratic equation,
   
   \f$a x^2 + b x + c = 0\f$

   The number of real roots (either zero, one or two) is returned, and
   their locations are stored in @a x0 and @a x1. If no real roots are found
   then @a x0 and @a x1 are not modified. If one real root is found (i.e. if
   a=0) then it is stored in @a x0. When two real roots are found they
   are stored in @a x0 and @a x1 in ascending order. The case of coincident
   roots is not considered special. For example \f$(x-1)^2=0\f$ will have
   two roots, which happen to have exactly equal values.

   The number of roots found depends on the sign of the discriminant
   \f$b^2 - 4 a c\f$. This will be subject to rounding and
   cancellation errors when computed in double precision, and will
   also be subject to errors if the coefficients of the polynomial are
   inexact. These errors may cause a discrete change in the number of
   roots. However, for polynomials with small integer coefficients the
   discriminant can always be computed exactly.

   @param[in] a
   @param[in] b
   @param[in] c
   @param[out] x0 root value 1 (if found)
   @param[out] x1 root value 2 (if found)

 */
int QGSLPolynomials :: solveQuadratic (double a, double b, double c, double &x0, double &x1)
{
   return gsl_poly_solve_quadratic(a, b, c, &x0, &x1);
}

GSL_END_NAMESPACE
PORTO_END_NAMESPACE
