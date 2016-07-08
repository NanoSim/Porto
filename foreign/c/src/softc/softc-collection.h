#ifndef SOFTC_COLLECTION_H_DEF
#define SOFTC_COLLECTION_H_DEF

#include "softc-macros.h"

SOFTC_BEGIN_DECLS

typedef struct _softc_collection_s softc_collection_s;

softc_collection_s * softc_collection_create(const char *id);
softc_collection_s * softc_collection_create_ext(const char *name, const char *version);
void                 softc_collection_free(softc_collection_s *self);
void                 softc_collection_register_entity(softc_collection_s *self, const char *label, const softc_entity_t *entity);
void                 softc_collection_add_dim(softc_collection_s *self, const char *label, const char *description);
void                 softc_collection_connection(softc_collection_s *self, const char *subject, const char *predicate, const char *object);
size_t               softc_collection_num_entities(softc_collection_s *self);
size_t               softc_collection_num_dims(softc_collection_s *self);
size_t               softc_collection_num_relations(softc_collection_s *self);
size_t               softc_collection_num_dim_maps(softc_collection_s *self);
void                 softc_collection_get_dimensions(softc_collection_s *self, char ***dimensions);

SOFTC_END_DECLS

#endif /* SOFTC_COLLECTION_H_DEF */
