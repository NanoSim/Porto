#include <QVariantList>
#include <QVector>
#include <gsl/gsl_fit.h>
#include "qgsllinearregression.h"

PORTO_BEGIN_NAMESPACE
GSL_BEGIN_NAMESPACE

#include "qgslutils.hh"

static size_t const stride = 1;

QGSLLinearRegression :: QGSLLinearRegression (QObject *parent)
   : QObject (parent)
{}

QGSLLinearRegression :: ~QGSLLinearRegression()
{}

int QGSLLinearRegression :: linear (QVariantList const &data1, QVariantList const &data2, double &c0, double &c1, double &cov00, double &cov01, double &cov11, double &sumsq)
{
   auto xs = toVector<double>(data1);
   auto ys = toVector<double>(data2);
   return gsl_fit_linear (xs.constData(), stride, ys.constData(), stride, xs.count(), &c0, &c1, &cov00, &cov01, &cov11, &sumsq);
}

int QGSLLinearRegression :: wlinear (QVariantList const &xdata, QVariantList const &wdata, QVariantList const &ydata, double &c0, double &c1, double &cov00, double &cov01, double &cov11, double &chisq)
{
   auto xs = toVector<double>(xdata);
   auto ws = toVector<double>(wdata);
   auto ys = toVector<double>(ydata);
   return gsl_fit_wlinear(xs.constData(), stride, ws.constData(), stride, ys.constData(), stride, xs.count(), &c0, &c1, &cov00, &cov01, &cov11, &chisq);
}

int QGSLLinearRegression :: linearEst (double x, double c0, double c1, double cov00, double cov01, double cov11, double &y, double &yerr)
{
   return gsl_fit_linear_est (x, c0, c1, cov00, cov01, cov11, &y, &yerr);
}

int QGSLLinearRegression :: mul (QVariantList const &xdata,QVariantList const &ydata, double &c1, double &cov11, double &sumsq)
{
   auto xs = toVector<double>(xdata);
   auto ys = toVector<double>(ydata);

   return gsl_fit_mul(xs.constData(), stride, ys.constData(), stride, xs.count(), &c1, &cov11, &sumsq);
}

int QGSLLinearRegression :: wmul (QVariantList const &xdata, QVariantList const &wdata, QVariantList const &ydata, double &c1, double &cov11, double &sumsq)
{
   auto xs = toVector<double>(xdata);   
   auto ws = toVector<double>(wdata);
   auto ys = toVector<double>(ydata);
   return gsl_fit_wmul(xs.constData(), stride, ws.constData(), stride, ys.constData(), stride, xs.count(), &c1, &cov11, &sumsq);
}


GSL_END_NAMESPACE
PORTO_END_NAMESPACE
