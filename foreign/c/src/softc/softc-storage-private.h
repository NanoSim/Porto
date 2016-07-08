#ifndef SOFTC_STORAGE_PRIVATE_H_DEF
#define SOFTC_STORAGE_PRIVATE_H_DEF

#include "softc-storage.h"
#include "softc-storage-strategy.h"
#include "softc-macros.h"

SOFTC_BEGIN_DECLS

softc_storage_t *softc_storage_private_create(const char *driver, const char *uri, const char *options);
void softc_storage_private_free(softc_storage_t *storage);
void softc_storage_private_load(softc_storage_t *storage, softc_entity_t *entity);
void softc_storage_private_save(softc_storage_t *storage, const softc_entity_t *entity);
softc_storage_strategy_t * softc_storage_private_get_storage_strategy(softc_storage_t *storage);
void softc_storage_private_free_storage_strategy(softc_storage_strategy_t *strategy);

SOFTC_END_DECLS
#endif
