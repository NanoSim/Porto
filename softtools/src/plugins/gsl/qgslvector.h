#ifndef QGSLVECTOR_H_DEF
#define QGSLVECTOR_H_DEF

#include <QObject>
#include <QVariantList>
#include <memory>
#include <gsl/gsl_vector.h>
#include "gslns.h"

SOFT_BEGIN_NAMESPACE
GSL_BEGIN_NAMESPACE

class QGSLVector : public QObject
{
   Q_OBJECT
public:
   explicit QGSLVector (QObject *parent = nullptr);
   QGSLVector (gsl_vector *vector, QObject *parent = nullptr);
   QGSLVector (size_t i, QObject *parent = nullptr);
   QGSLVector (QVariantList const &xs, QObject *parent = nullptr);
   QGSLVector (const QGSLVector & other);
   QGSLVector (QGSLVector && other);
   virtual ~QGSLVector();
   
public slots:
   QVariantList toArray() const;
   void   set          (int idx, double value) const;
   double get          (int idx) const;
   void   setAll       (double value);
   void   setZero      ();
   int    setBasis     (int i);
   int    swapElements (int i, int j);
   int    reverse      ();
   int    add          (soft::gsl::QGSLVector *other);
   int    sub          (soft::gsl::QGSLVector *other);
   int    mul          (soft::gsl::QGSLVector *other);   
   int    div          (soft::gsl::QGSLVector *other);
   int    scale        (double factor);
   int    addConstant  (double value);
   double max() const;
   double min() const;
   int    maxIndex() const;
   int    minIndex() const;
   
   int isNull() const;
   int isPos() const;
   int isNeg() const;
   int isNonNeg() const;
   static bool equal(soft::gsl::QGSLVector *u, soft::gsl::QGSLVector *v);

public:
   void minmax(double &min_out, double &max_out) const;
   void minMaxIndex(size_t &imin, size_t &imax) const;

protected:
   gsl_vector *data() const;
private:
   std::shared_ptr<gsl_vector> vector;
};

GSL_END_NAMESPACE
SOFT_END_NAMESPACE

#endif // QGSLVECTOR_H_DEF
