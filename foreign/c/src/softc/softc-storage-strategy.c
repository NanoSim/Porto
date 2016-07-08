#include "softc-storage-strategy.h"
#include "softc-storage-strategy-private.h"

softc_datamodel_t * softc_storage_strategy_get_datamodel(softc_storage_strategy_t *self)
{
  return softc_storage_strategy_private_get_datamodel(self);
}

void softc_storage_strategy_free_datamodel(softc_datamodel_t *datamodel)
{
  softc_storage_strategy_private_free_datamodel(datamodel);
}

void softc_storage_strategy_store(softc_storage_strategy_t *s, const softc_datamodel_t *m)
{
  softc_storage_strategy_private_store(s, m);
}

void softc_storage_strategy_start_retrieve(softc_storage_strategy_t *s, softc_datamodel_t *m)
{
  softc_storage_strategy_private_start_retrieve(s, m);
}

void softc_storage_strategy_end_retrieve(softc_storage_strategy_t *s, softc_datamodel_t *m)
{
  softc_storage_strategy_private_end_retrieve(s, m);
}
