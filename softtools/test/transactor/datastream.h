#ifndef DATASTREAM_H_DEF
#define DATASTREAM_H_DEF

#include "softns.h"

class StorageStrategy;

NAMESPACE_SOFT_BEGIN

class DataStream
{
public:
  explicit DataStream (StorageStrategy *d);
  virtual ~DataStream();

  DataStream &operator << (double);
  
private:
  class Private;
  Private *d;
};

NAMESPACE_SOFT_END

#endif // DATASTREAM_H_DEF
