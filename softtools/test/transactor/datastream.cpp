#include <QScopedPointer>
#include "storagestrategy.h"
#include "datastream.h"

NAMESPACE_SOFT_BEGIN

class DataStream :: Private
{
  friend class DataStream;
  Private (StorageStrategy *ptr)
    : strategy(ptr)
  {}

  StorageStrategy *strategy;
};

DataStream :: DataStream (StorageStrategy *ptr)
  : d (new DataStream::Private(ptr))
{}

DataStream :: ~DataStream()
{
  delete d;
}

DataStream &DataStream :: operator << (double value)
{
  d->strategy->store(value);
  return *this;
}

NAMESPACE_SOFT_END
