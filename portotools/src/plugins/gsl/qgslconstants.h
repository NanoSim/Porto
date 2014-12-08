#ifndef QGSLCONSTANTS_H_DEF
#define QGSLCONSTANTS_H_DEF

#include <QObject>
#include "gslns.h"

PORTO_BEGIN_NAMESPACE
GSL_BEGIN_NAMESPACE

class QGSLConstants : public QObject
{
   Q_OBJECT
   Q_PROPERTY (double M_E        MEMBER m_E CONSTANT);
   Q_PROPERTY (double M_LOG2E    MEMBER m_LOG2E CONSTANT);
   Q_PROPERTY (double M_LOG10E   MEMBER m_LOG10E CONSTANT);
   Q_PROPERTY (double M_SQRT2    MEMBER m_SQRT2 CONSTANT);
   Q_PROPERTY (double M_SQRT1_2  MEMBER  m_SQRT1_2 CONSTANT);
   Q_PROPERTY (double M_SQRT3    MEMBER  m_MSQRT3 CONSTANT);
   Q_PROPERTY (double M_PI       MEMBER  m_PI CONSTANT);
   Q_PROPERTY (double M_PI_2     MEMBER  m_PI_2 CONSTANT);
   Q_PROPERTY (double M_PI_4     MEMBER  m_PI_4 CONSTANT);
   Q_PROPERTY (double M_SQRTPI   MEMBER  m_SQRTPI CONSTANT);
   Q_PROPERTY (double M_2_SQRTPI MEMBER  m_2_SQRTPI CONSTANT);
   Q_PROPERTY (double M_1_PI     MEMBER  m_1_PI CONSTANT);
   Q_PROPERTY (double M_2_PI     MEMBER  m_2_PI CONSTANT);
   Q_PROPERTY (double M_LN10     MEMBER  m_LN10 CONSTANT);
   Q_PROPERTY (double M_LN2      MEMBER  m_LN2 CONSTANT);
   Q_PROPERTY (double M_LNPI     MEMBER  m_LNPI CONSTANT);
   Q_PROPERTY (double M_EULER    MEMBER  m_EULER CONSTANT);

public:
   explicit QGSLConstants (QObject *parent = nullptr);
   virtual ~QGSLConstants();

public slots:
   static bool isnan (const double x);
   static bool isinf (const double x);
   static bool finite (const double x);

   static double log1p (const double x);
   static double expm1 (const double x);
   static double hypot (const double x, const double y);
   static double hypot3 (const double x, const double y, const double z);
   static double acosh (const double x);
   static double asinh (const double x);
   static double atanh (const double x);
   static double ldexp (double x, int e);


public:
   static double frexp (double x, int & e);

private:
   double const m_E;
   double const m_LOG2E;
   double const m_LOG10E;
   double const m_SQRT2;
   double const m_SQRT1_2;
   double const m_MSQRT3;
   double const m_PI;
   double const m_PI_2;
   double const m_PI_4;
   double const m_SQRTPI;
   double const m_2_SQRTPI;
   double const m_1_PI;
   double const m_2_PI;
   double const m_LN10;
   double const m_LN2;
   double const m_LNPI;
   double const m_EULER;

};

GSL_END_NAMESPACE
PORTO_END_NAMESPACE

#endif // QGSLCONSTANTS_H_DEF

