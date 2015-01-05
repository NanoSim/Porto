#ifndef QGSLPOLYNOMIALS_H_DEF
#define QGSLPOLYNOMIALS_H_DEF

#include <QObject>
#include "gslns.h"

SOFT_BEGIN_NAMESPACE
GSL_BEGIN_NAMESPACE

class QGSLPolynomials : public QObject
{
   Q_OBJECT
public:
   explicit QGSLPolynomials (QObject *parent = nullptr);
   virtual ~QGSLPolynomials();

			     
public:
   static int solveQuadratic (double a, double b, double c, double &x0, double &x1);
};

GSL_END_NAMESPACE
SOFT_END_NAMESPACE


#endif // QGSLPOLYNOMIALS_H_DEF
