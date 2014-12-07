
#include <QVariantList>
#include <QVector>
#include <gsl/gsl_statistics.h>
#include "qgslstatistics.h"

/**
  @headerfile <qgslstatistics.h>
  @title Statistical functions 
  @brief Basic statistical functions
  
  The basic statistical functions include routines to compute the mean,
  variance and standard deviation. More advanced functions allow you to
  calculate absolute deviations, skewness, and kurtosis as well as the
  median and arbitrary percentiles. The algorithms use recurrence
  relations to compute average quantities in a stable way, without
  large intermediate values that might overflow.

  All documentation is based on the documentation given on 
  https://www.gnu.org/software/gsl/
*/

PORTO_BEGIN_NAMESPACE
GSL_BEGIN_NAMESPACE
#include "qgslutils.hh"

static size_t const stride = 1;

QGSLStatistics :: QGSLStatistics (QObject *parent)
  : QObject(parent)
{}

QGSLStatistics :: ~QGSLStatistics()
{}

/**
  @brief Returns the arithmetic mean of @a data. 

  The arithmetic mean, or sample mean, is denoted by \f$\bar{\mu}\f$
  and defined as,
  
  \f$\bar{\mu} = (1/N) \sum x_i\f$
  
  where \f$x_i\f$ are the elements of the dataset @a data. For samples
  drawn from a gaussian distribution the variance of \f$\bar{\mu}\f$
  is \f$\sigma^2 / N\f$.
  
  @param data list of samples

 */
double QGSLStatistics :: mean (QVariantList const &data)
{
   auto xs = toVector<double>(data);
   return gsl_stats_mean(xs.constData(), stride, xs.count());
}

/**
   @brief Returns the estimated, or sample, variance of data

   This function returns the estimated, or sample, variance of @a data, a
   dataset of length n with stride stride. The estimated variance is
   denoted by \bar{\sigma}^2 and is defined by,
   
   \f$\bar{\sigma}^2 = (1/(N-1)) \sum (x_i - \bar{\mu})^2\f$ where
   \f$x_i\f$ are the elements of the dataset data. Note that the
   normalization factor of \f$1/(N-1)\f$ results from the derivation of
   \f$\bar{\sigma}^2\f$ as an unbiased estimator of the population
   variance \f$\sigma^2\f$. For samples drawn from a Gaussian
   distribution the variance of \f$\bar{\sigma}^2\f$ itself is \f$2
   \sigma^4 / N\f$.

   @param data list of samples

   \sa double QGSLStatistics :: variance (QVariantList const &data, double m)
*/

double QGSLStatistics :: variance (QVariantList const &data)
{
   auto xs = toVector<double>(data);
   return gsl_stats_variance(xs.constData(), stride, xs.count());
}

/**
   @brief An overloaded function

   This is an overloaded function. If you have already computed the
   mean \f$m\f$ then you can pass it directly to this function.

   \f$\bar{\sigma}^2 = (1/(N-1)) \sum (x_i - m)^2\f$

   @param data list of samples
   @param m arithmetic mean of the data
 */
double QGSLStatistics :: variance (QVariantList const &data, double m)
{
   auto xs = toVector<double>(data);
   return gsl_stats_variance_m(xs.constData(), stride, xs.count(), m);
}

/**
   @brief The standard deviation is defined as the square root of the variance.

   @param data sample data
   \sa variance
 */
double QGSLStatistics :: sd (QVariantList const &data)
{
   auto xs = toVector<double>(data);
   return gsl_stats_sd(xs.constData(), stride, xs.count());
}

/**
   @brief Overloaded function.

   @param data sample data
   \sa variance
 */

double QGSLStatistics :: sd (QVariantList const &data, double m)
{
   auto xs = toVector<double>(data);
   return gsl_stats_sd_m(xs.constData(), stride, xs.count(), m);
}

/**
   @brief This function return the total sum of squares (TSS) of data
   about the mean.
 */
double QGSLStatistics :: tss (QVariantList const &data)
{
   auto xs = toVector<double>(data);
   return gsl_stats_tss (xs.constData(), stride, xs.count());
}

/**
   @brief Overloaded function that use the given mean

   \f$TSS =  \sum (x_i - m)^2\f$
 */
double QGSLStatistics :: tss (QVariantList const &data, double m)
{
   auto xs = toVector<double>(data);  
   return gsl_stats_tss_m (xs.constData(), stride, xs.count(), m);
}

/**
   @brief Computes an unbiased estimate of the variance of data

   This function computes an unbiased estimate of the variance of data
   when the population mean @a m of the underlying distribution is
   known a priori. In this case the estimator for the variance uses
   the factor 1/N and the sample mean \bar{\mu} is replaced by the known
   population mean \mu,

   \f$\bar{\sigma}^2 = (1/N) \sum (x_i - \mu)^2\f$

   @param data sample data
   @param m mean

 */
double QGSLStatistics :: varianceWithFixedMean (QVariantList const &data, double m)
{
   auto xs = toVector<double>(data);  
   return gsl_stats_variance_with_fixed_mean (xs.constData(), stride, xs.count(), m);
}

/**
   @brief Calculates the standard deviation of @a data

   This function calculates the standard deviation of data for a fixed
   population mean mean. The result is the square root of the
   corresponding variance function.

   @param data sample data
   @param m mean 
 */
double QGSLStatistics :: sdWithFixedMean (QVariantList const &data, double m)
{
   auto xs = toVector<double>(data);  
   return gsl_stats_sd_with_fixed_mean (xs.constData(), stride, xs.count(), m);
}

/**
   @brief Computes the absolute deviation from the mean of @a data

   This function computes the absolute deviation from the mean of
   data, a dataset of length \f$n\f$ with stride stride. The absolute
   deviation from the mean is defined as,

   \f$absdev = (1/N) \sum |x_i - \bar{\mu}|\f$ 

   where \f$x_i\f$ are the elements of the dataset data. The absolute
   deviation from the mean provides a more robust measure of the width
   of a distribution than the variance

   @param data sample data
*/
double QGSLStatistics :: absdev (QVariantList const &data)
{
   auto xs = toVector<double>(data);  
   return gsl_stats_absdev(xs.constData(), stride, xs.count());
}

/**
   @brief Overloaded function using the given precalculated mean \f$m\f$

   This function computes the absolute deviation of the dataset data relative to the given value of mean,

   \f$absdev = (1/N) \sum |x_i - mean|\f$ 

   This function is useful if you have already computed the mean of
   data (and want to avoid recomputing it), or wish to calculate the
   absolute deviation relative to another value (such as zero, or the
   median).

   @param data sample data
   @param m mean
 */
double QGSLStatistics :: absdev (QVariantList const &data, double m)
{
   auto xs = toVector<double>(data);  
   return gsl_stats_absdev_m(xs.constData(), stride, xs.count(), m);
}

/**
   @brief Computes the skewness of data
   
   This function computes the skewness of data, a dataset of length
   n. The skewness is defined as,

   \f$skew = (1/N) \sum ((x_i - \bar{\mu})/\bar{\sigma})^3\f$ 

   where \f$x_i\f$ are the elements of the dataset data. The skewness
   measures the asymmetry of the tails of a distribution.

   @param data sample data
 */
double QGSLStatistics :: skew (QVariantList const &data)
{
   auto xs = toVector<double>(data);  
   return gsl_stats_skew(xs.constData(), stride, xs.count());
}

/**
   @brief Computes the skewness of the dataset data.

   This function computes the skewness of the dataset data using the
   given values of the mean mean and standard deviation sd,
   
   \f$skew = (1/N) \sum ((x_i - mean)/sd)^3\f$

   These functions are useful if you have already computed the mean
   and standard deviation of data and want to avoid recomputing them.

   @param data sample data
   @param m mean

 */
double QGSLStatistics :: skew (QVariantList const &data, double m, double sd)
{
   auto xs = toVector<double>(data);  
   return gsl_stats_skew_m_sd(xs.constData(), stride, xs.count(), m, sd);
}

/**
   @brief computes the kurtosis of data 

   This function computes the kurtosis of $a data, a dataset of length
   n. The kurtosis is defined as,

   \f$kurtosis = ((1/N) \sum ((x_i - \bar{\mu})/\bar{\sigma})^4)  - 3\f$

   The kurtosis measures how sharply peaked a distribution is,
   relative to its width. The kurtosis is normalized to zero for a
   Gaussian distribution.

   @param data sample data
 */
double QGSLStatistics :: kurtosis (QVariantList const &data)
{
   auto xs = toVector<double>(data);  
   return gsl_stats_kurtosis(xs.constData(), stride, xs.count());
}

/**
   @brief Computes the kurtosis of the dataset data
   
   This function computes the kurtosis of the dataset data using the
   given values of the mean mean and standard deviation sd,

   \f$kurtosis = ((1/N) \sum ((x_i - mean)/sd)^4) - 3\f$

   This function is useful if you have already computed the mean and
   standard deviation of data and want to avoid recomputing them.

   @param data sample data 
   @param m mean

 */
double QGSLStatistics :: kurtosis (QVariantList const &data, double m, double sd)
{
   auto xs = toVector<double>(data);  
   return gsl_stats_kurtosis_m_sd(xs.constData(), stride, xs.count(), m, sd);
}

/**
   @brief computes the lag-1 autocorrelation of a dataset

   This function computes the lag-1 autocorrelation of the dataset data.

   \f$a_1 = {\sum_{i = 1}^{n} (x_{i} - \bar{\mu}) (x_{i-1} - \bar{\mu})
   \over
   \sum_{i = 1}^{n} (x_{i} - \bar{\mu}) (x_{i} - \bar{\mu})}\f$

   \sa double QGSLStatistics :: lag1Autocorrelation (QVariantList const &data, double m)

   @param data sample data
 */
double QGSLStatistics :: lag1Autocorrelation (QVariantList const &data)
{
   auto xs = toVector<double>(data);  
   return gsl_stats_lag1_autocorrelation(xs.constData(), stride, xs.count());
}

/**
   @brief This function computes the lag-1 autocorrelation of the
   dataset data using the given value of the mean mean.

   @param data sample data
   @param m mean
 */
double QGSLStatistics :: lag1Autocorrelation (QVariantList const &data, double m)
{
   auto xs = toVector<double>(data);  
   return gsl_stats_lag1_autocorrelation_m(xs.constData(), stride, xs.count(), m);
}

/**
   @brief This function computes the covariance of the datasets data1
   and data2 which must both be of the same length n.

   \f$covar = (1/(n - 1)) \sum_{i = 1}^{n} (x_i - \bar{ x}) (y_i - \bar{ y})\f$

   @param data1 first dataset
   @param data2 second dataset

 */
double QGSLStatistics :: covariance (QVariantList const &data1, QVariantList const &data2)
{
   auto xs = toVector<double>(data1);
   auto ys = toVector<double>(data2);
   return gsl_stats_covariance(xs.constData(), stride, ys.constData(), stride, xs.count());
}

/**
   @brief Computes the covariance of two datasets

   This function computes the covariance of the datasets @a data1 and
   @a data2 using the given values of the means, @a m1 and @a m2. This is
   useful if you have already computed the means of @a data1 and @a data2
   and want to avoid recomputing them.
 */
double QGSLStatistics :: covariance (QVariantList const &data1, QVariantList const &data2, double m1, double m2)
{
   auto xs = toVector<double>(data1);
   auto ys = toVector<double>(data2);
   return gsl_stats_covariance_m(xs.constData(), stride, ys.constData(), stride, xs.count(), m1, m2);
}

/**
   @brief Computes the Pearson correlation coefficient

   This function efficiently computes the Pearson correlation
   coefficient between the datasets data1 and data2 which must both be
   of the same length n.

   \f$
   
r = cov(x, y) / (\bar{\sigma}_x \bar{\sigma}_y)
  = {1/(n-1) \sum (x_i - \bar{ x}) (y_i - \bar{ y})
     \over
     \sqrt{1/(n-1) \sum (x_i - \bar{ x})^2} \sqrt{1/(n-1) \sum (y_i - \bar{ y})^2}
    }
    \f$
 */
double QGSLStatistics :: correlation (QVariantList const &data1, QVariantList const &data2)
{
   auto xs = toVector<double>(data1);
   auto ys = toVector<double>(data2);
   return gsl_stats_correlation(xs.constData(), stride, ys.constData(), stride, xs.count());
}

/**
   @brief This function computes the Spearman rank correlation coefficient

   This function computes the Spearman rank correlation coefficient
   between the datasets data1 and data2 which must both be of the same
   length n. Additional workspace of size \f$2*n\f$ is required in work. The
   Spearman rank correlation between vectors \f$x\f$ and \f$y\f$ is equivalent to
   the Pearson correlation between the ranked vectors \f$x_R\f$ and \f$y_R\f$,
   where ranks are defined to be the average of the positions of an
   element in the ascending order of the values.
 */
double QGSLStatistics :: spearman (QVariantList const &data1, QVariantList const &data2)
{
   auto xs = toVector<double>(data1);
   auto ys = toVector<double>(data2);
   QVector<double> work(2*xs.count());

   return gsl_stats_spearman(xs.constData(), stride, ys.constData(), stride, xs.count(), work.data());
}

/**
   @brief Returns the weighted mean of a dataset
   
   This function returns the weighted mean of the dataset data with stride stride and length n, using the set of weights w with stride wstride and length n. The weighted mean is defined as,

   \f$\bar{\mu} = (\sum w_i x_i) / (\sum w_i)\f$   
 */

double QGSLStatistics :: wmean (QVariantList const &w, QVariantList const &data)
{
   auto weight = toVector<double>(w);
   auto xs = toVector<double>(data);
   return gsl_stats_wmean (weight.constData(), stride, xs.constData(), stride, xs.count());
}

/**
   @brief Returns the estimated variance of the dataset

   This function returns the estimated variance of the dataset
   data with stride stride and length n, using the set of weights w
   with stride wstride and length n. The estimated variance of a
   weighted dataset is calculated as,

   \f$\bar{\sigma}^2 = ((\sum w_i)/((\sum w_i)^2 - \sum (w_i^2))) 
                \sum w_i (x_i - \bar{\mu})^2\f$

   Note that this expression reduces to an unweighted variance with
   the familiar \f$1/(N-1)\f$ factor when there are N equal non-zero weights.
 */
double QGSLStatistics :: wvariance (QVariantList const &w, QVariantList const &data)
{
   auto weight = toVector<double>(w);
   auto xs = toVector<double>(data);
   return gsl_stats_wvariance (weight.constData(), stride, xs.constData(), stride, xs.count());
}

/**
   @brief This overloaded function returns the estimated variance of
   the weighted dataset data using the given weighted mean wmean.
 */
double QGSLStatistics :: wvariance (QVariantList const &w, QVariantList const &data, double m)
{
   auto weight = toVector<double>(w);
   auto xs = toVector<double>(data);
   return gsl_stats_wvariance_m (weight.constData(), stride, xs.constData(), stride, xs.count(), m);
}

/**
   The standard deviation is defined as the square root of the
   variance. This function returns the square root of the
   corresponding variance function @ref wvariance above.
*/
double QGSLStatistics :: wsd (QVariantList const &data1, QVariantList const &data2)
{
   auto weight = toVector<double>(data1);
   auto xs = toVector<double>(data2);
   return gsl_stats_wsd (weight.constData(), stride, xs.constData(), stride, xs.count());
}

/**
   @brief overloaded function
 */
double QGSLStatistics :: wsd (QVariantList const &data1, QVariantList const &data2, double m)
{
   auto weight = toVector<double>(data1);
   auto xs = toVector<double>(data2);
   return gsl_stats_wsd_m (weight.constData(), stride, xs.constData(), stride, xs.count(), m);
}

/**
   This function computes an unbiased estimate of the variance of the
   weighted dataset data when the population mean mean of the
   underlying distribution is known a priori. In this case the
   estimator for the variance replaces the sample mean \f$\bar{\mu}\f$ by the
   known population mean \f$\mu\f$,

   \f$\bar{\sigma}^2 = (\sum w_i (x_i - \mu)^2) / (\sum w_i)\f$

 */
double QGSLStatistics :: wvarianceWithFixedMean (QVariantList const &w, QVariantList const &data, double m)
{
   auto weight = toVector<double>(w);
   auto xs = toVector<double>(data);
   return gsl_stats_wvariance_with_fixed_mean (weight.constData(), stride, xs.constData(), stride, xs.count(), m);
}

/**
   The standard deviation is defined as the square root of the
   variance. This function returns the square root of the
   corresponding function @se wvarianceWithFixedMean above.
 */
double QGSLStatistics :: wsdWithFixedMean (QVariantList const &w, QVariantList const &data, double m)
{
   auto weight = toVector<double>(w);
   auto xs = toVector<double>(data);
   return gsl_stats_wsd_with_fixed_mean (weight.constData(), stride, xs.constData(), stride, xs.count(), m);
}

/**
   @brief This function return the weighted total sum of squares (TSS)
   of data about the weighted mean.

   \f$TSS =  \sum w_i (x_i - wmean)^2\f$
 */
double QGSLStatistics :: wtss (QVariantList const &w, QVariantList const &data)
{
   auto weight = toVector<double>(w);
   auto xs = toVector<double>(data);
   return gsl_stats_wtss (weight.constData(), stride, xs.constData(), stride, xs.count());
}

/**
   @brief Overloaded function 
 */
double QGSLStatistics :: wtss (QVariantList const &w, QVariantList const &data, double m)
{
   auto weight = toVector<double>(w);
   auto xs = toVector<double>(data);
   return gsl_stats_wtss_m (weight.constData(), stride, xs.constData(), stride, xs.count(), m);
}

/**
   This function computes the weighted absolute deviation from the
   weighted mean of data. The absolute deviation from the mean is
   defined as,

   \f$absdev = (\sum w_i |x_i - \bar{\mu}|) / (\sum w_i)\f$

 */
double QGSLStatistics :: wabsdev (QVariantList const &data1, QVariantList const &data2)
{
   auto weight = toVector<double>(data1);
   auto xs = toVector<double>(data2);
   return gsl_stats_wabsdev (weight.constData(), stride, xs.constData(), stride, xs.count());
}

/**
   @brief Overloaded function.

   This function computes the absolute deviation of the weighted
   dataset data about the given weighted mean wmean.
 */
double QGSLStatistics :: wabsdev (QVariantList const &w, QVariantList const &data, double m)
{
   auto weight = toVector<double>(w);
   auto xs = toVector<double>(data);
   return gsl_stats_wabsdev_m (weight.constData(), stride, xs.constData(), stride, xs.count(), m);
}

/**
   @brief This function computes the weighted skewness of the dataset data.

   \f$skew = (\sum w_i ((x_i - \bar{ x})/\bar{ \sigma})^3) / (\sum w_i)\f$

 */
double QGSLStatistics :: wskew (QVariantList const &w, QVariantList const &data)
{
   auto weight = toVector<double>(w);
   auto xs = toVector<double>(data);
   return gsl_stats_wskew (weight.constData(), stride, xs.constData(), stride, xs.count());
}

/**
   This function computes the weighted skewness of the dataset data using
   the given values of the weighted mean and weighted standard deviation,
   @sa wmean and @sa wsd.
 */
double QGSLStatistics :: wskew (QVariantList const &w, QVariantList const &data, double wm, double wsd)
{
   auto weight = toVector<double>(w);
   auto xs = toVector<double>(data);
   return gsl_stats_wskew_m_sd (weight.constData(), stride, xs.constData(), stride, xs.count(), wm, wsd);
}

/**
   @brief This function computes the weighted kurtosis of the dataset data.

   \f$kurtosis = ((\sum w_i ((x_i - \bar{ x})/\bar{ \sigma})^4) / (\sum w_i)) - 3\f$
 */
double QGSLStatistics :: wkurtosis (QVariantList const &w, QVariantList const &data)
{
   auto weight = toVector<double>(w);
   auto xs = toVector<double>(data);
   return gsl_stats_wkurtosis (weight.constData(), stride, xs.constData(), stride, xs.count());
}

/**
   @brief Overloaded function 
   This function computes the weighted kurtosis of the dataset data using
   the given values of the weighted mean and weighted standard deviation,
   wmean and wsd.
 */
double QGSLStatistics :: wkurtosis (QVariantList const &data1, QVariantList const &data2, double wm, double wsd)
{
   auto weight = toVector<double>(data1);
   auto xs = toVector<double>(data2);
   return gsl_stats_wkurtosis_m_sd (weight.constData(), stride, xs.constData(), stride, xs.count(), wm, wsd);
}

/**
   @brief Returns the maximum value in a dataset

   This function returns the maximum value in @a data, a dataset of
   length n. The maximum value is defined as the value of the element
   \f$x_i\f$ which satisfies \f$x_i >= x_j\f$ for all \f$j\f$.

   If you want instead to find the element with the largest absolute
   magnitude you will need to apply fabs or abs to your data before
   calling this function.
 */
double QGSLStatistics :: max (QVariantList const &data)
{
   auto xs = toVector<double>(data);
   return gsl_stats_max(xs.constData(), stride, xs.count());
}

/**
   @brief Returns the minimum value in a dataset
   
   This function returns the minimum value in @a data, a dataset of
   length n. The minimum value is defined as the value of the element
   \f$x_i\f$ which satisfies \f$x_i <= x_j\f$ for all \f$j\f$.

   If you want instead to find the element with the smallest absolute
   magnitude you will need to apply fabs or abs to your data before
   calling this function.
 */
double QGSLStatistics :: min (QVariantList const &data)
{
   auto xs = toVector<double>(data);
   return gsl_stats_min(xs.constData(), stride, xs.count());
}

/**
   @brief Returns the index of the maximum value in a dataset

   This function returns the index of the maximum value in @a data, a
   dataset of length n with. The maximum value is
   defined as the value of the element \f$x_i\f$ which satisfies
   \f$x_i >= x_j\f$ for all \f$j\f$. When there are several equal
   maximum elements then the first one is chosen.
 */
size_t QGSLStatistics :: maxIndex (QVariantList const &data)
{
   auto xs = toVector<double>(data);
   return gsl_stats_max_index(xs.constData(), stride, xs.count());
}

/**
   @brief Returns the index of the minimum value in a dataset

   This function returns the index of the minimum value in data, a
   dataset of length n. The minimum value is defined as the value of
   the element \f$x_i\f$ which satisfies \f$x_i >= x_j\f$ for all
   \f$j\f$. When there are several equal minimum elements then the
   first one is chosen.
 */
size_t QGSLStatistics :: minIndex (QVariantList const &data)
{
   auto xs = toVector<double>(data);
   return gsl_stats_min_index(xs.constData(), stride, xs.count());
}

/**
   @brief returns the median value of sorted data

   This function returns the median value of @a sortedData, a dataset
   of length n. The elements of the array must be in ascending
   numerical order. There are no checks to see whether the data are
   sorted, so the function @sa QGSL::sort should always be used first.

   When the dataset has an odd number of elements the median is the
   value of element \f$(n-1)/2\f$. When the dataset has an even number
   of elements the median is the mean of the two nearest middle
   values, elements \f$(n-1)/2\f$ and \f$n/2\f$. Since the algorithm
   for computing the median involves interpolation this function
   always returns a floating-point number, even for integer data
   types.
 */
double QGSLStatistics :: medianFromSortedData(QVariantList const &sortedData)
{
   auto xs = toVector<double>(sortedData);
   return gsl_stats_median_from_sorted_data(xs.constData(), stride, xs.count());
}

/**
   @brief Returns a quantile value a sorted dataset

   This function returns a quantile value of @a sortedData, a
   double-precision array of length. The elements of the array must be
   in ascending numerical order. The quantile is determined by the f,
   a fraction between 0 and 1. For example, to compute the value of
   the 75th percentile f should have the value 0.75.

   There are no checks to see whether the data are sorted, so the
   function sort should always be used first.

   The quantile is found by interpolation, using the formula

   \f$quantile = (1 - \delta) x_i + \delta x_{i+1}\f$

   where \f$i\f$ is \f$floor((n - 1)f)\f$ and \f$\delta is (n-1)f - i\f$.
 */
double QGSLStatistics :: quantileFromSortedData(QVariantList const &sortedData, double frac)
{
   auto xs = toVector<double>(sortedData);
   return gsl_stats_quantile_from_sorted_data(xs.constData(), stride, xs.count(), frac);
}


GSL_END_NAMESPACE
PORTO_END_NAMESPACE
