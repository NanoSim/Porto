#include <gsl/gsl_math.h>
#include "qgslelementary.h"

SOFT_BEGIN_NAMESPACE
GSL_BEGIN_NAMESPACE

QGSLElementary :: QGSLElementary (QObject *parent)
   : QObject (parent)
{}

QGSLElementary :: ~QGSLElementary()
{}

/**
   @brief This function computes the value of \f$\log(1+x)\f$ in a way
   that is accurate for small @a x. It provides an alternative to the
   BSD math function \f$log1p(x)\f$.
 */
double QGSLElementary :: log1p (const double x)
{
   return gsl_log1p(x);
}

/**
   @brief This function computes the value of \f$\exp(x)-1\f$ in a way
   that is accurate for small @a x. It provides an alternative to the
   BSD math function \f$expm1(x)\f$
 */
double QGSLElementary :: expm1 (const double x)
{
   return gsl_expm1(x);
}

/**
   @brief This function computes the value of \f$\sqrt{x^2 + y^2}\f$
   in a way that avoids overflow. It provides an alternative to the
   BSD math function hypot(x,y).
 */
double QGSLElementary :: hypot (const double x, const double y)
{
   return gsl_hypot(x, y);
}

/**
   @brief This function computes the value of \f$\sqrt{x^2 + y^2 +
   z^2}\f$ in a way that avoids overflow.
 */
double QGSLElementary :: hypot3 (const double x, const double y, const double z)
{
   return hypot3(x,y,z);
}

/**
   @brief This function computes the value of \f$arccosh(x)\f$. It
   provides an alternative to the standard math function acosh(x).
 */
double QGSLElementary :: acosh (const double x)
{
   return acosh(x);
}

/**
   @brief This function computes the value of \f$arcsinh(x)\f$. It
   provides an alternative to the standard math function asinh(x)
 */
double QGSLElementary :: asinh (const double x)
{
   return asinh(x);
}

/**
   @brief This function computes the value of \f$arctanh(x)\f$. It
   provides an alternative to the standard math function atanh(x).
 */
double QGSLElementary :: atanh (const double x)
{
   return atanh(x);
}

/**
   @brief This function computes the value of \f$x * 2^e\f$. It
   provides an alternative to the standard math function ldexp(x,e).
 */
double QGSLElementary :: ldexp (double x, int e)
{
   return ldexp(x, e);
}

/**
   @brief This function splits the number @a x into its normalized
   fraction \f$f\f$ and exponent \f$e\f$, such that \f$x = f * 2^e\f$
   and \f$0.5 <= f < 1\f$. The function returns \f$f\f$ and stores the
   exponent in @a e. If x is zero, both f and e are set to zero. This
   function provides an alternative to the standard math function
   frexp(x, e)
   
   @param[in] x
   @param[out] e
 */
double QGSLElementary :: frexp (double x, int & e)
{
   return gsl_frexp(x, &e);
}

GSL_END_NAMESPACE
SOFT_END_NAMESPACE
