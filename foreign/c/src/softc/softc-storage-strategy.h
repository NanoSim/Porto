#ifndef SOFTC_STORAGE_STRATEGY_H_DEF
#define SOFTC_STORAGE_STRATEGY_H_DEF

#include "softc-macros.h"
#include "softc-datamodel.h"
SOFTC_BEGIN_DECLS

typedef struct _softc_storage_strategy_t softc_storage_strategy_t;

softc_datamodel_t * softc_storage_strategy_get_datamodel(softc_storage_strategy_t *);
void softc_storage_strategy_free_datamodel(softc_datamodel_t *datamodel);
void                softc_storage_strategy_store(softc_storage_strategy_t *, const softc_datamodel_t *);
void                softc_storage_strategy_start_retrieve(softc_storage_strategy_t *, softc_datamodel_t *);
void                softc_storage_strategy_end_retrieve(softc_storage_strategy_t *, softc_datamodel_t *);

SOFTC_END_DECLS

#endif
