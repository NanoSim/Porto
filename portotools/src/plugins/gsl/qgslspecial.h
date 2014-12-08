#ifndef QGSLSPECIAL_H_DEF
#define QGSLSPECIAL_H_DEF

#include <QObject>
#include <gsl/gsl_sf_result.h>
#include "gslns.h"

PORTO_BEGIN_NAMESPACE
GSL_BEGIN_NAMESPACE

class QGSLSpecial : public QObject
{
   Q_OBJECT
public:
   explicit QGSLSpecial (QObject *parent = nullptr);
   virtual ~QGSLSpecial();

public slots:

public:   
   static double airyAi (double x, gsl_sf_result &result);
   static double airyBi (double x, gsl_sf_result &result);
   static double airyAiScaled (double x, gsl_sf_result &result);
   static double airyBiScaled (double x, gsl_sf_result &result);
   /* Airy functions */
};
GSL_END_NAMESPACE
PORTO_END_NAMESPACE


#endif // QGSLSPECIAL_H_DEF
