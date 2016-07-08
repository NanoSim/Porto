#include <cstdlib>
#include "storage.h"
#include "softc-storage.h"
#include "softc-entity.h"
#include "softc-storage-strategy.h"
#include "softc-storage-private.h"
#include "softc-storage-strategy-private.hpp"

struct _softc_storage_t
{
  soft::Storage *ref;
};

softc_storage_t *softc_storage_private_create(const char *driver, const char *uri, const char *options)
{
  softc_storage_t *storage;
  storage = (softc_storage_t *)malloc(sizeof *storage);
  storage->ref = new soft::Storage(driver, uri, options);
  return storage;
}

void softc_storage_private_free(softc_storage_t *storage)
{
  delete storage->ref;
  storage->ref = nullptr;
  free (storage);
}

void softc_storage_private_load(softc_storage_t *storage, softc_entity_t *entity)
{
  softc_storage_strategy_t * strategy = softc_storage_private_get_storage_strategy(storage);
  softc_datamodel_t * datamodel = softc_storage_strategy_get_datamodel(strategy);
  softc_datamodel_set_id(datamodel, softc_entity_get_id(entity));
  softc_datamodel_set_meta_name(datamodel, softc_entity_get_meta_name(entity));
  softc_datamodel_set_meta_version(datamodel, softc_entity_get_meta_version(entity));
  softc_datamodel_set_meta_namespace(datamodel, softc_entity_get_meta_namespace(entity));

  softc_storage_strategy_start_retrieve(strategy, datamodel);
  softc_entity_load (entity, datamodel);
  softc_storage_strategy_end_retrieve(strategy, datamodel);
}

void softc_storage_private_save(softc_storage_t *storage, const softc_entity_t *entity)
{
  softc_storage_strategy_t * strategy = softc_storage_private_get_storage_strategy(storage);
  softc_datamodel_t * datamodel = softc_storage_strategy_get_datamodel(strategy);
  softc_datamodel_set_id(datamodel, softc_entity_get_id(entity));
  softc_datamodel_set_meta_name(datamodel, softc_entity_get_meta_name(entity));
  softc_datamodel_set_meta_version(datamodel, softc_entity_get_meta_version(entity));
  softc_datamodel_set_meta_namespace(datamodel, softc_entity_get_meta_namespace(entity));
  softc_entity_store (entity, datamodel);
  softc_storage_strategy_store(strategy, datamodel);
}

softc_storage_strategy_t * softc_storage_private_get_storage_strategy(softc_storage_t *storage)
{
  softc_storage_strategy_t* storage_strategy = (softc_storage_strategy_t*)malloc(sizeof *storage_strategy);
  if (!storage_strategy)
    return nullptr;
  
  storage_strategy->ref = storage->ref->strategy();
  if (!storage_strategy->ref)
    return nullptr;

  return storage_strategy;
}

void softc_storage_private_free_storage_strategy(softc_storage_strategy_t *strategy)
{
  free(strategy);
}
