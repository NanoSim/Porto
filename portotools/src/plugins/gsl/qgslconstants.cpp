#include <QtScript>
#include "qgslconstants.h"
#include <gsl/gsl_math.h>

PORTO_BEGIN_NAMESPACE
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
PORTO_END_NAMESPACE
