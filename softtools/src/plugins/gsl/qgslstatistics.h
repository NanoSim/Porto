#ifndef QGSLSTATISTICS_H_DEF
#define QGSLSTATISTICS_H_DEF

#include <QObject>
#include "gslns.h"

SOFT_BEGIN_NAMESPACE
GSL_BEGIN_NAMESPACE

class QGSLStatistics : public QObject
{
  Q_OBJECT

public:
  QGSLStatistics (QObject *parent = nullptr);
  virtual ~QGSLStatistics();

public slots:
  /* Mean, Standard Deviation and Variance */
  static double mean (QVariantList const &data);
  static double variance (QVariantList const &data);
  static double variance (QVariantList const &data, double m);
  static double sd (QVariantList const &data);
  static double sd (QVariantList const &data, double m);
  static double tss (QVariantList const &data);
  static double tss (QVariantList const &data, double m);
  static double varianceWithFixedMean (QVariantList const &data, double m);
  static double sdWithFixedMean (QVariantList const &data, double m);

  /* Absolute deviation */
  static double absdev (QVariantList const &data);
  static double absdev (QVariantList const &data, double m);

  /* Higher moments (skewness and kurtosis) */
  static double skew (QVariantList const &data);
  static double skew (QVariantList const &data, double m, double sd);
  static double kurtosis (QVariantList const &data);
  static double kurtosis (QVariantList const &data, double m, double sd);

  /* Autocorrelation */
  static double lag1Autocorrelation (QVariantList const &data);
  static double lag1Autocorrelation (QVariantList const &data, double m);

  /* Covariance */
  static double covariance (QVariantList const &data1, QVariantList const &data2);
  static double covariance (QVariantList const &data1, QVariantList const &data2, double m1, double m2);

  /* Correlation */
  static double correlation (QVariantList const &data1, QVariantList const &data2);
  static double spearman (QVariantList const &data1, QVariantList const &data2);

  /* Weighted Samples */
  static double wmean (QVariantList const &data1, QVariantList const &data2);
  static double wvariance (QVariantList const &data1, QVariantList const &data2);
  static double wvariance (QVariantList const &data1, QVariantList const &data2, double m);
  static double wsd (QVariantList const &data1, QVariantList const &data2);
  static double wsd (QVariantList const &data1, QVariantList const &data2, double m);
  static double wvarianceWithFixedMean (QVariantList const &data1, QVariantList const &data2, double m);
  static double wsdWithFixedMean (QVariantList const &data1, QVariantList const &data2, double m);
  static double wtss (QVariantList const &data1, QVariantList const &data2);
  static double wtss (QVariantList const &data1, QVariantList const &data2, double m);
  static double wabsdev (QVariantList const &data1, QVariantList const &data2);
  static double wabsdev (QVariantList const &data1, QVariantList const &data2, double m);
  static double wskew (QVariantList const &data1, QVariantList const &data2);
  static double wskew (QVariantList const &data1, QVariantList const &data2, double wm, double wsd);
  static double wkurtosis (QVariantList const &data1, QVariantList const &data2);
  static double wkurtosis (QVariantList const &data1, QVariantList const &data2, double wm, double wsd);

  /* Maximum and Minimum values */
  static double max (QVariantList const &data);
  static double min (QVariantList const &data);
  static size_t maxIndex (QVariantList const &data);
  static size_t minIndex (QVariantList const &data);

  /* Median and Percentiles */
  static double medianFromSortedData(QVariantList const &data);
  static double quantileFromSortedData(QVariantList const &data, double f);
};

GSL_END_NAMESPACE
SOFT_END_NAMESPACE

#endif // QGSLSTATISTICS_H_DEF
