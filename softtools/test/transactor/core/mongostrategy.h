#ifndef MONGOSTRATEGY_H_DEF
#define MONGOSTRATEGY_H_DEF

#include "storagestrategy.h"

class MongoStrategy : public StorageStrategy
{
public:
  MongoStrategy();
  virtual ~MongoStrategy();

  virtual bool store (double);
};

#endif // MONGOSTRATEGY_H_DEF
