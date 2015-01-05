#include <QVariantList>
#include <QVector>
#include <gsl/gsl_fit.h>
#include "qgsllinearregression.h"

/**
   @class QGSLLinearRegression
   @brief Library for simple one- or two-parameter regression and multiple-parameter fits.   
   
   Least Square Fitting
   ====================

   This class contains routines for performing least squares fits to
   experimental data using linear combinations of functions. The data
   may be weighted or unweighted, i.e. with known or unknown
   errors. For weighted data the functions compute the best fit
   parameters and their associated covariance matrix. For unweighted
   data the covariance matrix is estimated from the scatter of the
   points, giving a variance-covariance matrix.

   JavaScript Usage Example
   ------------------------
   @code{.js}
   var x = [1970, 1980, 1990, 2000],
       y = [  12,   11,   14,   13],
       w = [ 0.1,  0.2,  0.3,  0.4],
       n = x.length;

   var fn = gsl.fit.wlinear (x, w, y, function(c0, c1, cov00, cov01, cov11, chisq) {
       print ("best fit: Y = ", c0, "+", c1, "x");
   });

   var value = fn(1985);
   print ("y(1985) = ", value);
   @endcode


 */

SOFT_BEGIN_NAMESPACE
GSL_BEGIN_NAMESPACE

#include "qgslutils.hh"

static size_t const stride = 1;

QGSLLinearRegression :: QGSLLinearRegression (QObject *parent)
   : QObject (parent)
{}

QGSLLinearRegression :: ~QGSLLinearRegression()
{}

/**
   @brief Computes the best-fit linear regression coefficients

   This function computes the best-fit linear regression coefficients
   \f$(c0,c1)\f$ of the model \f$Y = c_0 + c_1 X\f$ for the dataset
   \f$(x, y)\f$, two vectors of length n. The errors on \f$y\f$ are
   assumed unknown so the variance-covariance matrix for the
   parameters \f$(c0, c1)\f$ is estimated from the scatter of the
   points around the best-fit line and returned via the parameters
   \f$(cov00, cov01, cov11)\f$. The sum of squares of the residuals
   from the best-fit line is returned in @a sumsq

   @param[in] data1
   @param[in] data2
   @param[out] c0
   @param[out] c1
   @param[out] cov00
   @param[out] cov01
   @param[out] cov1
   @param[out] sumsq
 */
int QGSLLinearRegression :: linear (QVariantList const &data1, QVariantList const &data2, double &c0, double &c1, double &cov00, double &cov01, double &cov11, double &sumsq)
{
   auto xs = toVector<double>(data1);
   auto ys = toVector<double>(data2);
   return gsl_fit_linear (xs.constData(), stride, ys.constData(), stride, xs.count(), &c0, &c1, &cov00, &cov01, &cov11, &sumsq);
}

/**
   @brief Computes the best-fit linear regression coefficients
   This function computes the best-fit linear regression coefficients
   \f$(c0,c1)\f$ of the model \f$Y = c_0 + c_1 X\f$ for the weighted
   dataset \f$(x, y)\f$, two vectors of length n. The vector \f$w\f$,
   of length n, specifies the weight of each datapoint. The weight is
   the reciprocal of the variance for each datapoint in \f$y\f$.

   The covariance matrix for the parameters \f$(c0, c1)\f$ is computed
   using the weights and returned via the parameters (@a cov00, @a cov01,
   @a cov11). The weighted sum of squares of the residuals from the
   best-fit line, \f$\chi^2\f$, is returned in @a chisq.
 */
int QGSLLinearRegression :: wlinear (QVariantList const &xdata, QVariantList const &wdata, QVariantList const &ydata, double &c0, double &c1, double &cov00, double &cov01, double &cov11, double &chisq)
{
   auto xs = toVector<double>(xdata);
   auto ws = toVector<double>(wdata);
   auto ys = toVector<double>(ydata);
   return gsl_fit_wlinear(xs.constData(), stride, ws.constData(), stride, ys.constData(), stride, xs.count(), &c0, &c1, &cov00, &cov01, &cov11, &chisq);
}

/**
   @brief compute a fittet function

   This function uses the best-fit linear regression coefficients c0,
   c1 and their covariance cov00, cov01, cov11 to compute the fitted
   function y and its standard deviation y_err for the model \f$Y = c_0 +
   c_1 X\f$ at the point \f$x\f$.
 */
int QGSLLinearRegression :: linearEst (double x, double c0, double c1, double cov00, double cov01, double cov11, double &y, double &yerr)
{
   return gsl_fit_linear_est (x, c0, c1, cov00, cov01, cov11, &y, &yerr);
}

/**
   @brief Computes the best-fit linear regression coefficient

   This function computes the best-fit linear regression coefficient
   @a c1 of the model \f$Y = c_1 X\f$ for the datasets (@a xdata, @a
   ydata), two vectors of length \f$n\f$. The errors on y are assumed
   unknown so the variance of the parameter @a c1 is estimated from the
   scatter of the points around the best-fit line and returned via the
   parameter @a cov11. The sum of squares of the residuals from the
   best-fit line is returned in @a sumsq.


 */
int QGSLLinearRegression :: mul (QVariantList const &xdata,QVariantList const &ydata, double &c1, double &cov11, double &sumsq)
{
   auto xs = toVector<double>(xdata);
   auto ys = toVector<double>(ydata);

   return gsl_fit_mul(xs.constData(), stride, ys.constData(), stride, xs.count(), &c1, &cov11, &sumsq);
}

/**
   @brief Computes the best-fit linear regression coefficient
   This function computes the best-fit linear regression coefficient
   @a c1 of the model \f$Y = c_1 X\f$ for the weighted datasets (@a
   xdata, @a ydata), two vectors of length n. The vector @a wdata, of
   length \f$n\f$, specifies the weight of each datapoint. The weight
   is the reciprocal of the variance for each datapoint in @a ydata.

   The variance of the parameter @a c1 is computed using the weights and
   returned via the parameter @a cov11. The weighted sum of squares of
   the residuals from the best-fit line, \f$\chi^2\f$, is returned in @a chisq.
 */
int QGSLLinearRegression :: wmul (QVariantList const &xdata, QVariantList const &wdata, QVariantList const &ydata, double &c1, double &cov11, double &sumsq)
{
   auto xs = toVector<double>(xdata);   
   auto ws = toVector<double>(wdata);
   auto ys = toVector<double>(ydata);
   return gsl_fit_wmul(xs.constData(), stride, ws.constData(), stride, ys.constData(), stride, xs.count(), &c1, &cov11, &sumsq);
}


GSL_END_NAMESPACE
SOFT_END_NAMESPACE
