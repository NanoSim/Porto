#include <functional>
#include <QMap>
#include <QString>
#include <QTextStream>
#include <QDebug>

#include "storagefactory.h"

SOFT_BEGIN_NAMESPACE

class StorageFactory::Private
{
  friend class StorageFactory;
  using MapType = QMap<QString, StorageFactory::CreateFunc>;
  MapType map;
};

/*!
  Constructs the storage factory
 */
StorageFactory :: StorageFactory()
   : d (new StorageFactory::Private())
{}

/*!
  Destroys the storage factory
 */

StorageFactory :: ~StorageFactory()
{
   delete d;
}

/*!
  Register a strategy with the given \a name and a create callback function \createFunc
 */
bool StorageFactory :: registerStrategy(const char *name, StorageFactory::CreateFunc createFunc)
{
   d->map[name] = createFunc;
   return true;
}

/*!
  Creates a storage strategy based on a given \a name. The \a uri and
  \a options are passed to the constructor of the IStorageStrategy

  \sa IStorageStrategy
 */
IStorageStrategy* StorageFactory :: create(const char *name, const char *uri, const char *options) const
{
  auto it = d->map.find(name);
  if(it == d->map.end()) {
    // @TODO Better error handling here -- this will simply crash
    QTextStream(stderr) << "StorageFactory cannot create storage strategy: " << name << " " << uri << endl;
    return nullptr;
  }
  
  CreateFunc createFunc = *it;
  return (*createFunc)(uri, options);
}

/*!
  Returns the number of currently registered drivers
 */
int StorageFactory :: driverCount() const
{
  return d->map.count();
}

/*!
  Return the driver name with index \a n
 */
std::string StorageFactory :: driverName(int n) const
{
  if (n >= 0 && d->map.count() > n) {
    auto it = d->map.constBegin();
    auto name = (it+n).key();
    return name.toStdString();
  }
  return std::string();
}


SOFT_END_NAMESPACE
