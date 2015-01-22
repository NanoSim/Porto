#include <mongoc.h>
#include <QSharedPointer>
#include "qbsonvalue.h"

NAMESPACE_SOFT_BEGIN

static QSharedPointer <bson_value_t> d_copy(const bson_value_t *src)
{
  auto dst = QSharedPointer <bson_value_t>::create();
  bson_value_copy (src, dst.data());
  return dst;
}

class QBsonValue::Private
{
  friend class QBsonValue;
  Private (QSharedPointer <bson_value_t> value)
    : value (value)
  {}

  QSharedPointer <bson_value_t> value;
};

QBsonValue :: QBsonValue (QObject *parent)
  : QObject (parent)
  , d (nullptr)
{}

QBsonValue :: QBsonValue (QBsonValue && other)
  : QObject (nullptr)
  , d(q_check_ptr(new QBsonValue::Private (other.d->value)))
{}

QBsonValue :: QBsonValue (_bson_value_t const * ptr, QObject *parent)
  : QObject (parent)
  , d (q_check_ptr(new QBsonValue::Private (d_copy(reinterpret_cast<const bson_value_t*>(ptr)))))
{}

QBsonValue :: ~QBsonValue()
{
  delete d;
}

int QBsonValue :: type() const
{
  return d->value->value_type;
}

double QBsonValue :: asDouble() const
{
  return d->value->value.v_double;
}

const char *QBsonValue::asConstCharPtr() const
{
  return const_cast<const char*>(d->value->value.v_utf8.str);
}

NAMESPACE_SOFT_END
