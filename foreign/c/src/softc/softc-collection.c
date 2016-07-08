#include <stdlib.h>
#include <string.h>
#include "softc.h"
#include "softc-datamodel.h"
#include "softc-entity.h"

#include "softc-collection.h"
/*#include "softc-collection-private.h"*/

static const char SOFTC_COLLECTION_META_TYPE[] = "collection:0.1:http://sintef.no/TR/metadata";
static const char SOFTC_COLLECTION_META_NAME[] = "collection";
static const char SOFTC_COLLECTION_META_VERSION[] = "1.0-SNAPSHOT-2";
static const char SOFTC_COLLECTION_META_NAMESPACE[] = "http://sintef.no/TR/metadata/simcoflow";

typedef struct _softc_collection_dimensions_s softc_collection_dimensions_s;
typedef struct _softc_collection_properties_s softc_collection_properties_s;

struct _softc_collection_dimensions_s
{};

struct _softc_collection_properties_s
{};

struct _softc_collection_s {
  const struct softc_entity_vtable_ *vtable_;
  const char *id;
  softc_collection_dimensions_s dims;
  softc_collection_properties_s props;
};
 
static const char * get_meta_type(const softc_entity_t *ptr)
{
  return SOFTC_COLLECTION_META_TYPE;
}

static const char * get_meta_name(const softc_entity_t *ptr)
{
  return SOFTC_COLLECTION_META_NAME;
}

static const char * get_meta_namespace(const softc_entity_t *ptr)
{
  return SOFTC_COLLECTION_META_NAMESPACE;
}

static const char * get_meta_version(const softc_entity_t *ptr)
{
  return SOFTC_COLLECTION_META_VERSION;
}

static void store (const softc_entity_t *ptr, softc_datamodel_t *data_model)
{}

static void load (softc_entity_t *ptr, const softc_datamodel_t *data_model)
{}

static const char ** get_dimensions(const softc_entity_t *ptr, size_t *size)
{
  return NULL;
}

static int get_dimension_size(const softc_entity_t *ptr, const char *label)
{
  return -1;
}

softc_collection_s * softc_collection_create(const char *id)
{
  SOFTC_ENTITY_VTABLE(softc_collection);
  softc_collection_s *self;
  self = malloc (sizeof *self);
  *self = (softc_collection_s) {SOFTC_ENTITY_VTABLE_NAME(softc_collection),
				softc_uuidgen(),
				(softc_collection_dimensions_s)
				{},
				(softc_collection_properties_s)
				{}
  };
  return self;
}

void softc_collection_free(softc_collection_s *self)
{
  free ((char*)self->id);
  free (self);
}

void softc_collection_register_entity(softc_collection_s *self, const char *label, const softc_entity_t *entity)
{
}

void softc_collection_add_dim(softc_collection_s *self, const char *label, const char *description)
{}

void softc_collection_connection(softc_collection_s *self, const char *subject, const char *predicate, const char *object)
{}

size_t softc_collection_num_entities(softc_collection_s *self)
{
  return 0;
}

size_t softc_collection_num_dims(softc_collection_s *self)
{
  return 0;
}
size_t softc_collection_num_relations(softc_collection_s *self)
{
  return 0;
}

size_t softc_collection_num_dim_maps(softc_collection_s *self)
{
  return 0;
}

void softc_collection_get_dimensions(softc_collection_s *self, char ***dimensions)
{  
}
