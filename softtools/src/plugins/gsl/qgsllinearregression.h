#ifndef QGSLLINEARREGRESSION_H_DEF
#define QGSLLINEARREGRESSION_H_DEF

#include <QObject>
#include "gslns.h"

SOFT_BEGIN_NAMESPACE
GSL_BEGIN_NAMESPACE

class QGSLLinearRegression : public QObject
{
   Q_OBJECT

public:
   explicit QGSLLinearRegression (QObject *parent = nullptr);
   virtual ~QGSLLinearRegression();

   static int linear (QVariantList const &xs, QVariantList const &ys, double &c0, double &c1, double &cov00, double &cov01, double &cov11, double &sumsq);
   static int wlinear (QVariantList const &xs, QVariantList const &ws, QVariantList const &ys, double &c0, double &c1, double &cov00, double &cov01, double &cov11, double &chisq);
   static int linearEst (double x, double c0, double c1, double cov00, double cov01, double cov11, double &y, double &yerr);
   static int mul (QVariantList const &xs,QVariantList const &ys, double &c1, double &cov11, double &sumsq);
   static int wmul (QVariantList const &xs, QVariantList const &ws, QVariantList const &ys, double &c1, double &cov11, double &sumsq);
};

GSL_END_NAMESPACE
SOFT_END_NAMESPACE

#endif // QGSLLINEARREGRESSION_H_DEF
