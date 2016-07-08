#ifndef QGSLMATRIX_H_DEF
#define QGSLMATRIX_H_DEF

#include <QObject>
#include <QVariantList>
#include <memory>
#include <gsl/gsl_matrix.h>
#include "gslns.h"

SOFT_BEGIN_NAMESPACE
GSL_BEGIN_NAMESPACE

class QGSLMatrix : public QObject
{ 
   Q_OBJECT
public:
   explicit QGSLMatrix (QObject *parent = nullptr);
   QGSLMatrix (gsl_matrix *matrix, QObject *parent = nullptr);
   QGSLMatrix (size_t i, size_t j, QObject *parent = nullptr);
   QGSLMatrix (QVariantList const &xs, QObject *parent = nullptr);
   QGSLMatrix (const QGSLMatrix & other);
   QGSLMatrix (QGSLMatrix && other);
   virtual ~QGSLMatrix();

   static gsl_matrix *variantListToMatrix (QVariantList const &xs);
   static QVariantList matrixToVariantList (gsl_matrix *m);
public slots:

   QVariantList toArray() const;
   double get(uint i, uint j) const;
   void set(uint i, uint j, double x);
   void setAll(double x);
   void setZero();
   void setIdentity();

   int transpose();
protected:
   gsl_matrix *data() const;

private:
   std::shared_ptr <gsl_matrix> matrix;
};

GSL_END_NAMESPACE
SOFT_END_NAMESPACE

#endif // QGSLMATRIX_H_DEF
