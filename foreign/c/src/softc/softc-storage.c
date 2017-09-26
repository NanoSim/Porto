#include <stdio.h>
#include "softc-storage.h"
#include "softc-storage-private.h"

softc_storage_t * softc_storage_create (const char *driver, const char *uri, const char *options)
{
  softc_storage_t *ret;
  ret = softc_storage_private_create(driver, uri, options);
  return ret;
}

void softc_storage_free (softc_storage_t *storage)
{
  softc_storage_private_free(storage);
}

void softc_storage_load (softc_storage_t *storage, softc_entity_t *entity)
{
  softc_storage_private_load(storage, entity);
}

void softc_storage_save (softc_storage_t *storage, const softc_entity_t *entity)
{
  softc_storage_private_save(storage,entity);
}

softc_storage_strategy_t * softc_storage_get_storage_strategy(softc_storage_t *storage)
{
  return softc_storage_private_get_storage_strategy(storage);
}

void softc_storage_free_storage_strategy(softc_storage_strategy_t *strategy)
{
  softc_storage_private_free_storage_strategy(strategy);
}
