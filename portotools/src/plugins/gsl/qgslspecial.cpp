/**
   @class QGSLSpecial   
   @brief Library for calculating the values of special functions

   The Special functions include Airy functions, Bessel functions,
   Clausen functions, Coulomb wave functions, Coupling coefficients,
   the Dawson function, Debye functions, Dilogarithms, Elliptic
   integrals, Jacobi elliptic functions, Error functions, Exponential
   integrals, Fermi-Dirac functions, Gamma functions, Gegenbauer
   functions, Hypergeometric functions, Laguerre functions, Legendre
   functions and Spherical Harmonics, the Psi (Digamma) Function,
   Synchrotron functions, Transport functions, Trigonometric functions
   and Zeta functions.

   Airy Functions and Derivatives
   ------------------------------
   The Airy functions Ai(x) and Bi(x) are defined by the integral representations,

   \f$Ai(x) = (1/\pi) \int_0^\infty \cos((1/3) t^3 + xt) dt
   Bi(x) = (1/\pi) \int_0^\infty (e^(-(1/3) t^3 + xt) + \sin((1/3) t^3 + xt)) dt\f$
   
   For further information see Abramowitz & Stegun, Section 10.4
*/

#include <QVariantList>
#include <QVector>
#include <gsl/gsl_sf.h>

#include "qgslspecial.h"

PORTO_BEGIN_NAMESPACE
GSL_BEGIN_NAMESPACE

QGSLSpecial :: QGSLSpecial (QObject *parent)
   : QObject (parent)
{}

QGSLSpecial :: ~QGSLSpecial()
{}

static const gsl_mode_t gslMode = GSL_PREC_DOUBLE;

/**
   @brief Computes the Airy function \f$Ai(x)\f$ with value/error field

   @param[in] x
   @param[out] result
 */
double QGSLSpecial :: airyAi (double x, gsl_sf_result &result)
{
   return gsl_sf_airy_Ai_e (x, gslMode, &result);
}

/**
   @brief Computes the Airy function \f$Bi(x)\f$ with value/error field

   @param[in] x
   @param[out] result

 */
double QGSLSpecial :: airyBi (double x, gsl_sf_result &result)
{
   return gsl_sf_airy_Bi_e (x, gslMode, &result);
}

/**
   @brief Compute a scaled version of the Airy function \f$S_A(x)
   Ai(x)\f$. 

   For \f$x>0\f$ the scaling factor \f$S_A(x)\f$ is \f$\exp(+(2/3)
   x^(3/2))\f$, and is \f$1\f$ for \f$x<0\f$.

   @param[in] x
   @param[out] result

 */
double QGSLSpecial :: airyAiScaled (double x, gsl_sf_result &result)
{
   return gsl_sf_airy_Ai_scaled_e (x, gslMode, &result);
}

/**
   @brief Compute a scaled version of the Airy function \f$S_B(x) Bi(x)\f$. 

   For \f$x>0\f$ the scaling factor \f$S_B(x)\f$ is \f$exp(-(2/3) x^(3/2))\f$, and is \f$1\f$ for \f$x<0\f$.

   PortoShell example
   ------------------

   @code{.js}
   var aiS = gsl.sf.airyAiScaled(1.2);

   // -> 0.2549279405913302

   gsl.sf.airyAiScaled(1.1, function(value, err) {
     print("value :", value);
     print("error :", err);
   });

   // value : 0.25905231799038403
   // error : 7.32627109029673e-17

   @endcode

   @param[in] x
   @param[out] result

 */
double QGSLSpecial :: airyBiScaled (double x, gsl_sf_result &result)
{
   return gsl_sf_airy_Bi_scaled_e (x, gslMode, &result);
}

GSL_END_NAMESPACE
PORTO_END_NAMESPACE
