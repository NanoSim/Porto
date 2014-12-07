#include "qgslmatrix.h"

PORTO_BEGIN_NAMESPACE
GSL_BEGIN_NAMESPACE

/**
   @class QGSLMatrix
   @brief A class representing a Matrix datatype
 */

gsl_matrix *QGSLMatrix :: variantListToMatrix (QVariantList const &xs)
{   
   auto size1 = xs.size();
   auto size2 = xs.first().toList().size();
   auto mtx = gsl_matrix_alloc(size1, size2);
   for (auto i = 0; i < size1; ++i) {
      auto xsInner = xs[i].toList();
      auto ptr = gsl_matrix_ptr(mtx, i, 0);
      auto it = xsInner.constBegin();
      while (it != xsInner.constEnd()) {
	 *(ptr++) = (*(it++)).value<double>();
      }
   }
   
   return mtx;
}

QVariantList QGSLMatrix :: matrixToVariantList (gsl_matrix *m)
{
   QVariantList variantList;
   for (size_t i = 0; i < m->size1; ++i) {
      QVariantList inner;
      for (size_t j = 0; j < m->size2; ++j) {
	 auto value = gsl_matrix_get(m, i, j);
	 inner << QVariant(value);
      }
      variantList << inner;
   }
   return variantList;
}

static void matrixDeleter (gsl_matrix *ptr)
{
   gsl_matrix_free (ptr);
}

QVariantList QGSLMatrix :: toArray() const
{
   return QGSLMatrix::matrixToVariantList(matrix.get());
}

QGSLMatrix :: QGSLMatrix (QObject *parent)
   : QObject (parent)
   , matrix (nullptr)
{}

QGSLMatrix :: QGSLMatrix (size_t i, size_t j, QObject *parent)
   : QObject (parent)
   , matrix (gsl_matrix_alloc(i, j), matrixDeleter)
{}

QGSLMatrix :: QGSLMatrix (gsl_matrix *matrix, QObject *parent)
   : QObject (parent)
   , matrix (matrix, matrixDeleter)
{}

QGSLMatrix :: QGSLMatrix (QVariantList const &xs, QObject *parent)
   : QObject (parent)
   , matrix (variantListToMatrix(xs), matrixDeleter)
{}

QGSLMatrix :: QGSLMatrix (const QGSLMatrix & other)
   : QObject (other.parent())
   , matrix (other.matrix)
{}

QGSLMatrix :: QGSLMatrix (QGSLMatrix && other)
   : QObject (std::move (other.parent()))
   , matrix (std::move (other.matrix))
{}

QGSLMatrix :: ~QGSLMatrix()
{}

gsl_matrix *QGSLMatrix :: data() const
{
   return matrix.get();
}

double QGSLMatrix :: get(uint i, uint j) const
{   
   return gsl_matrix_get(matrix.get(), i, j);
}

void QGSLMatrix :: set(uint i, uint j, double x)
{
   gsl_matrix_set(matrix.get(), i, j, x);
}

void QGSLMatrix :: setAll(double x)
{
   gsl_matrix_set_all(matrix.get(), x);
}

void QGSLMatrix :: setZero()
{
   gsl_matrix_set_zero(matrix.get());
}

void QGSLMatrix :: setIdentity()
{
   gsl_matrix_set_identity(matrix.get());
}

int QGSLMatrix :: transpose()
{
   return gsl_matrix_transpose(matrix.get());
}

GSL_END_NAMESPACE
PORTO_END_NAMESPACE
