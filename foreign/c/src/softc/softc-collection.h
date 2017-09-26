#ifndef SOFTC_COLLECTION_H_DEF
#define SOFTC_COLLECTION_H_DEF

#include <stddef.h>
#include "softc-entity.h"
#include "softc-string-list.h"
#include "softc-macros.h"

SOFTC_BEGIN_DECLS

typedef struct _softc_collection_s softc_collection_s;
typedef struct _softc_collection_allocatable_s softc_collection_allocatable_s;

typedef struct
{
  size_t E;
  size_t R;
} softc_collection_dimensions_s;

typedef struct _softc_collection_properties_s softc_collection_properties_s;

softc_collection_s *softc_collection_create_new();
softc_collection_s *softc_collection_create(const char *id);
softc_collection_s *softc_collection_create_ext(const char *name, const char *version);
void softc_collection_free(softc_collection_s *self);

softc_collection_properties_s * const softc_collection_props(softc_collection_s *self);
softc_collection_dimensions_s * const softc_collection_dims(softc_collection_s *self);
softc_collection_allocatable_s * softc_collection_allocatables(softc_collection_s *self);

void softc_collection_register_entity(softc_collection_s *self, const char *label, softc_entity_t *entity);
void softc_collection_find_entity(softc_collection_s *self, const char *label, softc_string_s name, softc_string_s version, softc_string_s ns, softc_string_s uuid);
size_t softc_collection_num_entities(softc_collection_s *self);
size_t softc_collection_num_relations(softc_collection_s *self);
softc_string_list_s *softc_collection_find_relations(softc_collection_s *self, const char *subject, const char *predicate);

void softc_collection_get_name(softc_collection_s *self, softc_string_s name);
void softc_collection_set_name(softc_collection_s *self, const char *name);
void softc_collection_set_version(softc_collection_s *self, const char *version);
void softc_collection_get_version(softc_collection_s *self, softc_string_s version);
void softc_collection_get_namespace(softc_collection_s *self, softc_string_s ns);
void softc_collection_set_namespace(softc_collection_s *self, const char *ns);
void softc_collection_add_relation(softc_collection_s *self, const char *subject, const char *predicate, const char *object);
void softc_collection_connect(softc_collection_s *self, const char *subject, const char *predicate, const char *object);


SOFTC_END_DECLS

#endif /* SOFTC_COLLECTION_H_DEF */
