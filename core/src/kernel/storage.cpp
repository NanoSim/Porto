#include "storage.h"
#include "soft.h"
#include "istoragestrategy.h"
#include "idatamodel.h"

SOFT_BEGIN_NAMESPACE

class Storage::Private
{
  friend class Storage;
  Private(char const *driver, char const *uri, const char *options)
    : strategy (createStrategy(driver, uri, options))
  {}
  
  ~Private()
  {
    delete strategy;
    strategy = nullptr;
  }
    
  IStorageStrategy *strategy;
};

/*!
  Constructs a Storage with a storage strategy given by the chosen
 \driver. The \a uri and \options are passed to the storage strategy.

  \sa IStorageStrategy IStoragePlugin
 */
Storage :: Storage(char const* driver, char const *uri, const char *options)
  : d (new Storage::Private(driver, uri, options))
{}

/*!
  Destroy the storage
 */
Storage :: ~Storage()
{
  delete d;
}

/*!
  Save an \a entity with the current storage strategy
 */
void Storage :: save (IEntity const *entity)
{
  IDataModel *dataModel = d->strategy->dataModel();
  dataModel->setId(entity->id());
  dataModel->setMetaName(entity->metaName());
  dataModel->setMetaVersion(entity->metaVersion());
  dataModel->setMetaNamespace(entity->metaNamespace());

  entity->save(dataModel);
  d->strategy->store(dataModel);
}

/*!
  Load an \a entity with the current storage strategy
  \sa IEntity
 */
void Storage :: load (IEntity *entity)
{
  auto dataModel = d->strategy->dataModel();
  dataModel->setId(entity->id());
  dataModel->setMetaName(entity->metaName());
  dataModel->setMetaVersion(entity->metaVersion());
  dataModel->setMetaNamespace(entity->metaNamespace());

  d->strategy->startRetrieve(dataModel);  
  entity->load(dataModel);
  d->strategy->endRetrieve(dataModel);
}

/*!
  Return the current storage strategy
  \sa IStorageStrategy
 */
IStorageStrategy *Storage :: strategy()
{
  if (d != nullptr && d->strategy != nullptr) {
    return d->strategy;
  }
  return nullptr;
}

SOFT_END_NAMESPACE
