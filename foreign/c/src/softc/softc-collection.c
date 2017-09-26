#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "softc.h"
#include "softc-entity.h"
#include "softc-datamodel.h"
#include "softc-allocatable.h"

#include "softc-collection.h"
#include "softc-collection-private.h"

static const char SOFTC_COLLECTION_META_TYPE[] = "Collection:1.0-RC-4:org.sintef.soft";
static const char SOFTC_COLLECTION_META_NAME[] = "Collection";
static const char SOFTC_COLLECTION_META_VERSION[] = "1.0-RC-4";
static const char SOFTC_COLLECTION_META_NAMESPACE[] = "org.sintef.soft";
static const char *static_dims[] = {"E","R"};

struct _softc_collection_properties_s
{
  softc_string_s __name__;
  softc_string_s __version__;
};

struct _softc_collection_allocatable_s
{
  short dummy;
};

typedef struct _softc_collection_private_s
{
  void *ptr;
} softc_collection_private_s;

struct _softc_collection_s {
  const struct softc_entity_vtable_ *vtable_;
  const char *id;
  softc_collection_dimensions_s dims;
  softc_collection_properties_s props;
  softc_collection_allocatable_s *allocs;
  softc_collection_private_s privat;
};

static void store (const softc_entity_t *ptr, softc_datamodel_t *data_model)
{
  const softc_collection_s *self = (const softc_collection_s*)ptr;
  softc_collection_private_save(self->privat.ptr, data_model);
  /*
  const softc_collection_s *self = (const softc_collection_s*)ptr;

  size_t E = self->dims.E;
  size_t R = self->dims.R;
  softc_datamodel_append_string (data_model, "__name__", self->props.__name__);
  softc_datamodel_append_string (data_model, "__version__", self->props.__version__);
  */
}

void softc_collection_register_entity(softc_collection_s *self, const char *label, softc_entity_t *entity)
{
  softc_collection_private_register_entity(self->privat.ptr, label, entity);
}

void softc_collection_find_entity( softc_collection_s *self
                                 , const char *label
                                 , softc_string_s name
                                 , softc_string_s version
                                 , softc_string_s ns
                                 , softc_string_s uuid)
{
  softc_collection_private_find_entity(self->privat.ptr, label, name, version, ns, uuid);
}

size_t softc_collection_num_entities(softc_collection_s *self)
{
  return (size_t) softc_collection_private_num_entities(self->privat.ptr);
}

static void load (softc_entity_t *ptr, const softc_datamodel_t *data_model)
{
  softc_collection_s *self = (softc_collection_s*)ptr;
  softc_collection_private_load(self->privat.ptr, data_model);

  /*
  softc_collection_s *self = (softc_collection_s*)ptr;

  softc_datamodel_get_string (data_model, "__name__", &self->props.__name__);
  softc_datamodel_get_string (data_model, "__version__", &self->props.__version__);
  */
}

static const char ** get_dimensions(const softc_entity_t *ptr, size_t *size)
{
  softc_collection_s *self = (softc_collection_s*) ptr;
  const size_t ndims = 2;
  size_t i;
  char **dl = malloc(sizeof (*dl) * ndims);
  dl[0] = malloc(sizeof(**dl) * 2);
  strcpy(dl[0], "E");
  dl[1] = malloc(sizeof(**dl) * 2);
  strcpy(dl[1], "R");
  *size = 2;
  return (const char**)dl;
}

static int get_dimension_size(const softc_entity_t *ptr, const char *label)
{
  softc_collection_s *self = (softc_collection_s*)ptr;
  if (strcmp(label, "E") == 0) return self->dims.E;
  if (strcmp(label, "R") == 0) return self->dims.R;
  assert(false); /* Illegal label */
  return 0;
}

static const char * get_meta_type()
{
  return SOFTC_COLLECTION_META_TYPE;
}

static const char * get_meta_name()
{
  return SOFTC_COLLECTION_META_NAME;
}

static const char * get_meta_namespace()
{
  return SOFTC_COLLECTION_META_NAMESPACE;
}

static const char * get_meta_version()
{
  return SOFTC_COLLECTION_META_VERSION;
}

static char* copy_string(const char *str)
{
  if (str) {
    char *c = (char*)malloc(strlen(str)+1);
    strcpy(c, str);
    return c;
  }
  return NULL;
}

softc_collection_s *softc_collection_create_ext(const char *name, const char *version)
{
  assert(false);
}

softc_collection_s *softc_collection_create_new()
{
  SOFTC_ENTITY_VTABLE(softc_collection);
  softc_collection_s *self;
  self = malloc (sizeof *self);
  const char *id = softc_uuidgen();
  softc_collection_allocatable_s *alloc = malloc(sizeof *alloc);
  *self = (softc_collection_s) {SOFTC_ENTITY_VTABLE_NAME(softc_collection),
				id,
				(softc_collection_dimensions_s)
				{0,0},
				(softc_collection_properties_s)
				{0,
				 0},
				(softc_collection_allocatable_s*){
				  alloc
				},
				(softc_collection_private_s)
				{
				  softc_collection_private_create(id)
				}
  };
  return self;
}

softc_collection_s *softc_collection_create(const char *id)
{
  if (!id) return softc_collection_create_new();
  SOFTC_ENTITY_VTABLE(softc_collection);
  softc_collection_s *self;
  self = malloc (sizeof *self);
  *self = (softc_collection_s) {SOFTC_ENTITY_VTABLE_NAME(softc_collection),
				(const char *)copy_string(id),
				(softc_collection_dimensions_s)
				{0,0},
				(softc_collection_properties_s)
				{0,
				 0},
				(softc_collection_allocatable_s*){
				  NULL
				},
				(softc_collection_private_s)
				{
				  softc_collection_private_create(id)
				}
  };
  return self;
}

void softc_collection_free(softc_collection_s *self)
{
  free((char*)self->id);
  free (self);
}

softc_collection_properties_s
*const softc_collection_props (softc_collection_s *self)
{
  return &self->props;
}

softc_collection_dimensions_s
*const softc_collection_dims (softc_collection_s *self)
{
  return &self->dims;
}

softc_collection_allocatable_s
*softc_collection_allocatables(softc_collection_s *self)
{
  return self->allocs;
}

size_t softc_collection_num_relations(softc_collection_s *self)
{
  return (size_t) softc_collection_private_num_relations(self->privat.ptr);
}

softc_string_list_s *softc_collection_find_relations(softc_collection_s *self,
                                                     const char *subject,
                                                     const char *predicate)
{
  return softc_collection_private_find_relations(self->privat.ptr,
                                                 subject,
                                                 predicate);
}


void softc_collection_get_name(softc_collection_s *self, softc_string_s name)
{
  softc_collection_private_get_name(self->privat.ptr, name);
}

void softc_collection_set_name(softc_collection_s *self, const char *name)
{
  softc_collection_private_set_name(self->privat.ptr, name);
}

void softc_collection_get_version(softc_collection_s *self, softc_string_s version)
{
  softc_collection_private_get_version(self->privat.ptr, version);
}

void softc_collection_set_version(softc_collection_s *self, const char *version)
{
  softc_collection_private_set_version(self->privat.ptr, version);
}

void softc_collection_get_namespace(softc_collection_s *self, softc_string_s ns)
{
  softc_collection_private_get_namespace(self->privat.ptr, ns);
}

void softc_collection_set_namespace(softc_collection_s *self, const char *ns)
{
  softc_collection_private_set_namespace(self->privat.ptr, ns);
}

void softc_collection_add_relation(softc_collection_s *self, const char *subject, const char *predicate, const char *object)
{
  softc_collection_private_add_relation(self->privat.ptr, subject, predicate, object);
}

void softc_collection_connect(softc_collection_s *self, const char *subject, const char *predicate, const char *object)
{
  softc_collection_private_connect(self->privat.ptr, subject, predicate, object);
}
