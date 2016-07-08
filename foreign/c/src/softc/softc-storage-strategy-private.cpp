#include <cstdlib>

#include "softc-datamodel.h"
#include "softc-datamodel-private.hpp"
#include "softc-storage-strategy-private.h"
#include "softc-storage-strategy-private.hpp"

softc_datamodel_t * softc_storage_strategy_private_get_datamodel(softc_storage_strategy_t *self)
{
  softc_datamodel_t *datamodel;
  datamodel = (softc_datamodel_t *)malloc(sizeof *datamodel);
  datamodel->ref = self->ref->dataModel();
  return datamodel;
}

void softc_storage_strategy_private_free_datamodel(softc_datamodel_t *datamodel)
{
  free(datamodel);
}

void softc_storage_strategy_private_store(softc_storage_strategy_t *self,
					  const softc_datamodel_t *model)
{
  self->ref->store(model->ref);
}

void softc_storage_strategy_private_start_retrieve(softc_storage_strategy_t *self,
						   softc_datamodel_t *model)
{
  self->ref->startRetrieve(model->ref);
}

void softc_storage_strategy_private_end_retrieve(softc_storage_strategy_t *self,
						 softc_datamodel_t *model)
{
  self->ref->endRetrieve(model->ref);
}
