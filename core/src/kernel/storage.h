#ifndef STORAGE_H_DEF
#define STORAGE_H_DEF

#include "softns.h"
#include "ientity.h"

SOFT_BEGIN_NAMESPACE

/*!
  class Storage is a context class for the IStorageStrategy
 */
class IStorageStrategy;
class Storage
{
public:
  Storage(char const *driver, char const *uri, const char *options = nullptr);
  virtual ~Storage();

  void save (IEntity const *);
  void load (IEntity *);

  IStorageStrategy *strategy();
private:
  class Private;
  Private *d;
};

SOFT_END_NAMESPACE

#endif // STORAGE_H_DEF
