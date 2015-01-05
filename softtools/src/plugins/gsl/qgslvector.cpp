#include "qgslvector.h"

SOFT_BEGIN_NAMESPACE
GSL_BEGIN_NAMESPACE

/**
   @class QGSLVector
   @brief A class representing a Vector datatype
 */

static gsl_vector *variantListToVector (QVariantList const &xs)
{
   auto v = gsl_vector_alloc(xs.size());
   auto it = xs.constBegin();
//   auto ptr = v->data;
   auto ptr = gsl_vector_ptr(v, 0); //->data;
   while (it != xs.constEnd()) {
      *(ptr++) = (*(it++)).value<double>();
   }
   
   return v;
}

static void vectorDeleter (gsl_vector *ptr)
{
   gsl_vector_free(ptr);
}

QGSLVector :: QGSLVector (QObject *parent)
   : QObject (parent)
   , vector (nullptr)
{}

QGSLVector :: QGSLVector(gsl_vector *vector, QObject *parent)
   : QObject (parent)
   , vector (vector, vectorDeleter)
{}

QGSLVector :: QGSLVector (size_t i, QObject *parent)
   : QObject (parent)
   , vector (gsl_vector_alloc(i), vectorDeleter)
{}

QGSLVector :: QGSLVector(QVariantList const &xs, QObject *parent)
   : QObject (parent)
   , vector (variantListToVector(xs), vectorDeleter)
{}

QGSLVector :: QGSLVector(QGSLVector const & other)
   : QObject (other.parent())
   , vector (other.vector)
{}

QGSLVector :: QGSLVector(QGSLVector && other)
   : QObject (std::move (other.parent()))
   , vector (std::move (other.vector))
{}

QVariantList QGSLVector :: toArray() const
{
   QVariantList variantList;
   auto v = vector.get();
   for (size_t idx = 0; idx < v->size; ++idx) {
      auto value = gsl_vector_get(v, idx);
      variantList << QVariant(value);
   }
   return variantList;
}

double QGSLVector :: get(int idx) const
{
   return gsl_vector_get (vector.get(), idx);
}

void QGSLVector :: set(int idx, double value) const
{
   gsl_vector_set (vector.get(), idx, value);
}

void QGSLVector :: setAll (double value)
{
   gsl_vector_set_all (vector.get(), value);
}

void QGSLVector :: setZero ()
{
   gsl_vector_set_zero (vector.get());
}

int QGSLVector :: setBasis (int i)
{
   return gsl_vector_set_basis (vector.get(), i);
}

int QGSLVector :: swapElements (int i, int j)
{
   return gsl_vector_swap_elements (vector.get(), i, j);
}

int QGSLVector :: reverse ()
{
   return gsl_vector_reverse (vector.get());
}

gsl_vector *QGSLVector :: data() const
{
   return vector.get();
}

int QGSLVector :: add (QGSLVector *other)
{
   return gsl_vector_add(vector.get(), other->vector.get());
}

int QGSLVector :: sub (QGSLVector *other)
{
   return gsl_vector_sub(vector.get(), other->vector.get());
}

int QGSLVector :: mul (QGSLVector *other)
{
   return gsl_vector_mul(vector.get(), other->vector.get());
}

int QGSLVector :: div (QGSLVector *other)
{
   return gsl_vector_div(vector.get(), other->vector.get());
}

int QGSLVector :: scale (double factor)
{
   return gsl_vector_scale(vector.get(), factor);
}

int QGSLVector :: addConstant (double value)
{
   return gsl_vector_add_constant(vector.get(), value);
}

double QGSLVector :: max() const
{
   return gsl_vector_max(vector.get());
}

double QGSLVector :: min() const
{
   return gsl_vector_min(vector.get());
}

int QGSLVector :: maxIndex() const
{
   return gsl_vector_max_index(vector.get());
}

int QGSLVector :: minIndex() const
{
   return gsl_vector_min_index(vector.get());
}

void QGSLVector :: minmax(double &min_out, double &max_out) const
{
   gsl_vector_minmax(vector.get(), &min_out, &max_out);
}

void QGSLVector :: minMaxIndex(size_t &imin, size_t &imax) const
{
   gsl_vector_minmax_index(vector.get(), &imin, &imax);
}

int QGSLVector :: isNull() const
{
   return gsl_vector_isnull(vector.get());
}

int QGSLVector :: isPos() const
{
   return gsl_vector_ispos(vector.get());
}

int QGSLVector :: isNeg() const
{
   return gsl_vector_isneg(vector.get());
}

int QGSLVector :: isNonNeg() const
{
   return gsl_vector_isnonneg(vector.get());
}

bool QGSLVector :: equal(soft::gsl::QGSLVector *u, soft::gsl::QGSLVector *v)
{
   return (gsl_vector_equal(u->data(), v->data()) > 0);
}


QGSLVector :: ~QGSLVector()
{}

GSL_END_NAMESPACE
SOFT_END_NAMESPACE
