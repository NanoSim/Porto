#include <QVariantList>
#include <QVector>
#include <gsl/gsl_statistics.h>
#include "qgslstatistics.h"

PORTO_BEGIN_NAMESPACE
GSL_BEGIN_NAMESPACE
#include "qgslutils.hh"

static size_t const stride = 1;

QGSLStatistics :: QGSLStatistics (QObject *parent)
  : QObject(parent)
{}

QGSLStatistics :: ~QGSLStatistics()
{}

double QGSLStatistics :: mean (QVariantList const &data)
{
   auto xs = toVector<double>(data);
   return gsl_stats_mean(xs.constData(), stride, xs.count());
}

double QGSLStatistics :: variance (QVariantList const &data)
{
   auto xs = toVector<double>(data);
   return gsl_stats_variance(xs.constData(), stride, xs.count());
}

double QGSLStatistics :: variance (QVariantList const &data, double m)
{
   auto xs = toVector<double>(data);
   return gsl_stats_variance_m(xs.constData(), stride, xs.count(), m);
}

double QGSLStatistics :: sd (QVariantList const &data)
{
   auto xs = toVector<double>(data);
   return gsl_stats_sd(xs.constData(), stride, xs.count());
}

double QGSLStatistics :: sd (QVariantList const &data, double m)
{
   auto xs = toVector<double>(data);
   return gsl_stats_sd_m(xs.constData(), stride, xs.count(), m);
}

double QGSLStatistics :: tss (QVariantList const &data)
{
   auto xs = toVector<double>(data);
   return gsl_stats_tss (xs.constData(), stride, xs.count());
}

double QGSLStatistics :: tss (QVariantList const &data, double m)
{
   auto xs = toVector<double>(data);  
   return gsl_stats_tss_m (xs.constData(), stride, xs.count(), m);
}

double QGSLStatistics :: varianceWithFixedMean (QVariantList const &data, double m)
{
   auto xs = toVector<double>(data);  
   return gsl_stats_variance_with_fixed_mean (xs.constData(), stride, xs.count(), m);
}

double QGSLStatistics :: sdWithFixedMean (QVariantList const &data, double m)
{
   auto xs = toVector<double>(data);  
   return gsl_stats_sd_with_fixed_mean (xs.constData(), stride, xs.count(), m);
}

double QGSLStatistics :: absdev (QVariantList const &data)
{
   auto xs = toVector<double>(data);  
   return gsl_stats_absdev(xs.constData(), stride, xs.count());
}

double QGSLStatistics :: absdev (QVariantList const &data, double m)
{
   auto xs = toVector<double>(data);  
   return gsl_stats_absdev_m(xs.constData(), stride, xs.count(), m);
}

double QGSLStatistics :: skew (QVariantList const &data)
{
   auto xs = toVector<double>(data);  
   return gsl_stats_skew(xs.constData(), stride, xs.count());
}

double QGSLStatistics :: skew (QVariantList const &data, double m, double sd)
{
   auto xs = toVector<double>(data);  
   return gsl_stats_skew_m_sd(xs.constData(), stride, xs.count(), m, sd);
}

double QGSLStatistics :: kurtosis (QVariantList const &data)
{
   auto xs = toVector<double>(data);  
   return gsl_stats_kurtosis(xs.constData(), stride, xs.count());
}

double QGSLStatistics :: kurtosis (QVariantList const &data, double m, double sd)
{
   auto xs = toVector<double>(data);  
   return gsl_stats_kurtosis_m_sd(xs.constData(), stride, xs.count(), m, sd);
}


double QGSLStatistics :: lag1Autocorrelation (QVariantList const &data)
{
   auto xs = toVector<double>(data);  
   return gsl_stats_lag1_autocorrelation(xs.constData(), stride, xs.count());
}

double QGSLStatistics :: lag1Autocorrelation (QVariantList const &data, double m)
{
   auto xs = toVector<double>(data);  
   return gsl_stats_lag1_autocorrelation_m(xs.constData(), stride, xs.count(), m);
}

double QGSLStatistics :: covariance (QVariantList const &data1, QVariantList const &data2)
{
   auto xs = toVector<double>(data1);
   auto ys = toVector<double>(data2);
   return gsl_stats_covariance(xs.constData(), stride, ys.constData(), stride, xs.count());
}

double QGSLStatistics :: covariance (QVariantList const &data1, QVariantList const &data2, double m1, double m2)
{
   auto xs = toVector<double>(data1);
   auto ys = toVector<double>(data2);
   return gsl_stats_covariance_m(xs.constData(), stride, ys.constData(), stride, xs.count(), m1, m2);
}

double QGSLStatistics :: correlation (QVariantList const &data1, QVariantList const &data2)
{
   auto xs = toVector<double>(data1);
   auto ys = toVector<double>(data2);
   return gsl_stats_correlation(xs.constData(), stride, ys.constData(), stride, xs.count());
}

double QGSLStatistics :: spearman (QVariantList const &data1, QVariantList const &data2)
{
   auto xs = toVector<double>(data1);
   auto ys = toVector<double>(data2);
   QVector<double> work(2*xs.count());

   return gsl_stats_spearman(xs.constData(), stride, ys.constData(), stride, xs.count(), work.data());
}

double QGSLStatistics :: wmean (QVariantList const &data1, QVariantList const &data2)
{
   auto weight = toVector<double>(data1);
   auto xs = toVector<double>(data2);
   return gsl_stats_wmean (weight.constData(), stride, xs.constData(), stride, xs.count());
}

double QGSLStatistics :: wvariance (QVariantList const &data1, QVariantList const &data2)
{
   auto weight = toVector<double>(data1);
   auto xs = toVector<double>(data2);
   return gsl_stats_wvariance (weight.constData(), stride, xs.constData(), stride, xs.count());
}

double QGSLStatistics :: wvariance (QVariantList const &data1, QVariantList const &data2, double m)
{
   auto weight = toVector<double>(data1);
   auto xs = toVector<double>(data2);
   return gsl_stats_wvariance_m (weight.constData(), stride, xs.constData(), stride, xs.count(), m);
}

double QGSLStatistics :: wsd (QVariantList const &data1, QVariantList const &data2)
{
   auto weight = toVector<double>(data1);
   auto xs = toVector<double>(data2);
   return gsl_stats_wsd (weight.constData(), stride, xs.constData(), stride, xs.count());
}

double QGSLStatistics :: wsd (QVariantList const &data1, QVariantList const &data2, double m)
{
   auto weight = toVector<double>(data1);
   auto xs = toVector<double>(data2);
   return gsl_stats_wsd_m (weight.constData(), stride, xs.constData(), stride, xs.count(), m);
}

double QGSLStatistics :: wvarianceWithFixedMean (QVariantList const &data1, QVariantList const &data2, double m)
{
   auto weight = toVector<double>(data1);
   auto xs = toVector<double>(data2);
   return gsl_stats_wvariance_with_fixed_mean (weight.constData(), stride, xs.constData(), stride, xs.count(), m);
}

double QGSLStatistics :: wsdWithFixedMean (QVariantList const &data1, QVariantList const &data2, double m)
{
   auto weight = toVector<double>(data1);
   auto xs = toVector<double>(data2);
   return gsl_stats_wsd_with_fixed_mean (weight.constData(), stride, xs.constData(), stride, xs.count(), m);
}

double QGSLStatistics :: wtss (QVariantList const &data1, QVariantList const &data2)
{
   auto weight = toVector<double>(data1);
   auto xs = toVector<double>(data2);
   return gsl_stats_wtss (weight.constData(), stride, xs.constData(), stride, xs.count());
}

double QGSLStatistics :: wtss (QVariantList const &data1, QVariantList const &data2, double m)
{
   auto weight = toVector<double>(data1);
   auto xs = toVector<double>(data2);
   return gsl_stats_wtss_m (weight.constData(), stride, xs.constData(), stride, xs.count(), m);
}


double QGSLStatistics :: wabsdev (QVariantList const &data1, QVariantList const &data2)
{
   auto weight = toVector<double>(data1);
   auto xs = toVector<double>(data2);
   return gsl_stats_wabsdev (weight.constData(), stride, xs.constData(), stride, xs.count());
}

double QGSLStatistics :: wabsdev (QVariantList const &data1, QVariantList const &data2, double m)
{
   auto weight = toVector<double>(data1);
   auto xs = toVector<double>(data2);
   return gsl_stats_wabsdev_m (weight.constData(), stride, xs.constData(), stride, xs.count(), m);
}


double QGSLStatistics :: wskew (QVariantList const &data1, QVariantList const &data2)
{
   auto weight = toVector<double>(data1);
   auto xs = toVector<double>(data2);
   return gsl_stats_wskew (weight.constData(), stride, xs.constData(), stride, xs.count());
}

double QGSLStatistics :: wskew (QVariantList const &data1, QVariantList const &data2, double wm, double wsd)
{
   auto weight = toVector<double>(data1);
   auto xs = toVector<double>(data2);
   return gsl_stats_wskew_m_sd (weight.constData(), stride, xs.constData(), stride, xs.count(), wm, wsd);
}


double QGSLStatistics :: wkurtosis (QVariantList const &data1, QVariantList const &data2)
{
   auto weight = toVector<double>(data1);
   auto xs = toVector<double>(data2);
   return gsl_stats_wkurtosis (weight.constData(), stride, xs.constData(), stride, xs.count());
}

double QGSLStatistics :: wkurtosis (QVariantList const &data1, QVariantList const &data2, double wm, double wsd)
{
   auto weight = toVector<double>(data1);
   auto xs = toVector<double>(data2);
   return gsl_stats_wkurtosis_m_sd (weight.constData(), stride, xs.constData(), stride, xs.count(), wm, wsd);
}

double QGSLStatistics :: max (QVariantList const &data)
{
   auto xs = toVector<double>(data);
   return gsl_stats_max(xs.constData(), stride, xs.count());
}

double QGSLStatistics :: min (QVariantList const &data)
{
   auto xs = toVector<double>(data);
   return gsl_stats_min(xs.constData(), stride, xs.count());
}

size_t QGSLStatistics :: maxIndex (QVariantList const &data)
{
   auto xs = toVector<double>(data);
   return gsl_stats_max_index(xs.constData(), stride, xs.count());
}

size_t QGSLStatistics :: minIndex (QVariantList const &data)
{
   auto xs = toVector<double>(data);
   return gsl_stats_min_index(xs.constData(), stride, xs.count());
}

double QGSLStatistics :: medianFromSortedData(QVariantList const &data)
{
   auto xs = toVector<double>(data);
   return gsl_stats_median_from_sorted_data(xs.constData(), stride, xs.count());
}

double QGSLStatistics :: quantileFromSortedData(QVariantList const &data, double frac)
{
   auto xs = toVector<double>(data);
   return gsl_stats_quantile_from_sorted_data(xs.constData(), stride, xs.count(), frac);
}


GSL_END_NAMESPACE
PORTO_END_NAMESPACE
