#ifndef SOFTC_COLLECTION_PRIVATE_H_DEF
#define SOFTC_COLLECTION_PRIVATE_H_DEF

#include <stddef.h>
#include "softc-macros.h"
#include "softc-entity.h"
#include "softc-string.h"
#include "softc-string-list.h"

SOFTC_BEGIN_DECLS

void *softc_collection_private_create(const char *id);
void softc_collection_private_register_entity(void *ref, const char *label, softc_entity_t *entity);
void softc_collection_private_find_entity(void *ref, const char *label, softc_string_s name, softc_string_s version, softc_string_s ns, softc_string_s uuid);
void softc_collection_private_set_name(void *ref, const char *name);
void softc_collection_private_get_name(void *ref, softc_string_s name);
void softc_collection_private_set_version(void *ref, const char *version);
void softc_collection_private_get_version(void *ref, softc_string_s version);
void softc_collection_private_set_namespace(void *ref, const char *ns);
void softc_collection_private_get_namespace(void *ref, softc_string_s ns);
void softc_collection_private_add_relation(void *ref, const char *subject, const char *predicate, const char *object);
void softc_collection_private_connect(void *ref, const char *subject, const char *predicate, const char *object);
int softc_collection_private_num_relations(void *ref);
int softc_collection_private_num_entities(void *ref);
void softc_collection_private_save(const void *ref, softc_datamodel_t *dataModel);
void softc_collection_private_load(void *ref, const softc_datamodel_t *dataModel);

softc_string_list_s *softc_collection_private_find_relations(void *ref, const char *subject, const char *predicate);


SOFTC_END_DECLS

#endif /* SOFTC_COLLECTION_PRIVATE_H_DEF */
