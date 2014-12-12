#ifndef QGSLELEMENTARY_H_DEF
#define QGSLELEMENTARY_H_DEF

#include <QObject>
#include "gslns.h"

PORTO_BEGIN_NAMESPACE
GSL_BEGIN_NAMESPACE

class QGSLElementary : public QObject
{
   Q_OBJECT

public:
   explicit QGSLElementary (QObject *parent = nullptr);
   virtual ~QGSLElementary();

public slots:
   static double log1p (const double x);
   static double expm1 (const double x);
   static double hypot (const double x, const double y);
   static double hypot3 (const double x, const double y, const double z);
   static double acosh (const double x);
   static double asinh (const double x);
   static double atanh (const double x);
   static double ldexp (double x, int e);
   static double frexp (double x, int & e);

};

GSL_END_NAMESPACE
PORTO_END_NAMESPACE

#endif // QGSLELEMENTARY_H_DEF
