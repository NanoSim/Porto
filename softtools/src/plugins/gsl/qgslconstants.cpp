#include <QtScript>
#include "qgslconstants.h"
#include <gsl/gsl_math.h>

/**
 @class QGSLConstants

 @brief The standard BSD mathematical constants

 The library ensures that the standard BSD mathematical
 constants are defined. For reference, here is a list of the
 constants:
 
 \li \c M_E The base of exponentials, \f$e\f$
 \li \c M_LOG2E The base-2 logarithm of \f$e\f$, \f$\log_2 (e)\f$
 \li \c M_LOG10E The base-10 logarithm of \f$e\f$, \f$\log_10 (e)\f$
 \li \c M_SQRT2 The square root of two, \f$\sqrt 2\f$
 \li \c M_SQRT1_2 The square root of one-half, \f$\sqrt{1/2}\f$
 \li \c M_SQRT3 The square root of three, \f$\sqrt 3\f$
 \li \c M_PI The constant pi, \f$\pi\f$
 \li \c M_PI_2 Pi divided by two, \f$\pi/2\f$
 \li \c M_PI_4 Pi divided by four, \f$\pi/4\f$
 \li \c M_SQRTPI The square root of pi, \f$\sqrt\pi\f$
 \li \c M_2_SQRTPI Two divided by the square root of pi, \f$2/\sqrt\pi\f$
 \li \c M_1_PI The reciprocal of pi, \f$1/\pi\f$
 \li \c M_2_PI Twice the reciprocal of pi, \f$2/\pi\f$
 \li \c M_LN10 The natural logarithm of ten, \f$\ln(10)\f$
 \li \c M_LN2 The natural logarithm of two, \f$\ln(2)\f$
 \li \c M_LNPI The natural logarithm of pi, \f$\ln(\pi)\f$
 \li \c M_EULER Euler’s constant, \f$\gamma\f$
*/

SOFT_BEGIN_NAMESPACE
GSL_BEGIN_NAMESPACE

QGSLConstants :: QGSLConstants (QObject *parent)
   : QObject (parent)
   , m_E (M_E)
   , m_LOG2E (M_LOG2E)
   , m_LOG10E (M_LOG10E)
   , m_SQRT2 (M_SQRT2)
   , m_SQRT1_2 (M_SQRT1_2)
   , m_MSQRT3 (M_SQRT3)
   , m_PI (M_PI)
   , m_PI_2 (M_PI_2)
   , m_PI_4 (M_PI_4)
   , m_SQRTPI (M_SQRTPI)
   , m_2_SQRTPI (M_2_SQRTPI)
   , m_1_PI (M_1_PI)
   , m_2_PI (M_2_PI)
   , m_LN10 (M_LN10)
   , m_LN2 (M_LN2)
   , m_LNPI (M_LNPI)
   , m_EULER (M_EULER)

{}

QGSLConstants :: ~QGSLConstants()
{}

bool QGSLConstants :: isnan (const double x)
{
   return gsl_isnan(x) == 1;
}

bool QGSLConstants :: isinf (const double x)
{
   return gsl_isinf (x) != 0;
}

bool QGSLConstants :: finite (const double x)
{
   return gsl_finite (x) == 1;
}

double QGSLConstants :: log1p (const double x)
{
   return gsl_log1p(x);
}

double QGSLConstants :: expm1 (const double x)
{
   return gsl_expm1(x);
}

double QGSLConstants :: hypot (const double x, const double y)
{
   return gsl_hypot(x,y);
}

double QGSLConstants :: hypot3 (const double x, const double y, const double z)
{
   return gsl_hypot3(x,y,z);
}

double QGSLConstants :: acosh (const double x)
{
   return gsl_acosh(x);
}

double QGSLConstants :: asinh (const double x)
{
   return gsl_asinh(x);
}

double QGSLConstants :: atanh (const double x)
{
   return gsl_atanh(x);
}

double QGSLConstants :: ldexp (double x, int e)
{
   return gsl_ldexp(x,e);
}

double QGSLConstants :: frexp (double x, int &e)
{
   return gsl_frexp(x,&e);
}


GSL_END_NAMESPACE
SOFT_END_NAMESPACE
