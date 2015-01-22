#ifndef STORAGESTRATEGY_H_DEF
#define STORAGESTRATEGY_H_DEF

class StorageStrategy
{
public:
  StorageStrategy();
  virtual ~StorageStrategy() = 0;

  virtual bool store (double) = 0;
  //  virtual StorageStrategy& operator<< (double) = 0;
};

#endif // STORAGESTRATEGY_H_DEF
