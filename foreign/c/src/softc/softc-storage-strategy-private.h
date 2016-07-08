#ifndef SOFTC_STORAGE_STRATEGY_PRIVATE_H_DEF
#define SOFTC_STORAGE_STRATEGY_PRIVATE_H_DEF

#include "softc-macros.h"
#include "softc-storage-strategy.h"
SOFTC_BEGIN_DECLS

softc_datamodel_t * softc_storage_strategy_private_get_datamodel(softc_storage_strategy_t *);
void softc_storage_strategy_private_free_datamodel(softc_datamodel_t *datamodel);
void softc_storage_strategy_private_store(softc_storage_strategy_t *, const softc_datamodel_t *);
void softc_storage_strategy_private_retrieve(softc_storage_strategy_t *, softc_datamodel_t *);
void softc_storage_strategy_private_start_retrieve(softc_storage_strategy_t *, softc_datamodel_t *);
void softc_storage_strategy_private_end_retrieve(softc_storage_strategy_t *, softc_datamodel_t *);

SOFTC_END_DECLS

#endif 
