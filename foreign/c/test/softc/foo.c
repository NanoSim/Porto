#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <softc/softc.h>
#include <softc/softc-entity.h>

#include "foo.h"

static const char FOO_META_TYPE[] = "foo:0.1:http://sintef.no/TR/metadata/simcoflow";
static const char FOO_META_NAME[] = "foo";
static const char FOO_META_VERSION[] = "0.1";
static const char FOO_META_NAMESPACE[] = "http://sintef.no/TR/metadata/simcoflow";

struct _foo_s {
  const struct softc_entity_vtable_ *vtable_;
  const char *id;
  foo_dimensions dims;
  foo_properties props;
};

static void store (const softc_entity_t *ptr, softc_datamodel_t *data_model)
{
  const foo_s *self = (const foo_s*)ptr;
  softc_string_s s;

  softc_datamodel_append_int32(data_model, "n", self->props.n);
  s = softc_string_create(self->props.str);
  softc_datamodel_append_string(data_model, "str", s);
  softc_datamodel_append_array_double(data_model, "array", self->props.array, self->dims.i_size);
  printf("storing...\n");
  softc_string_destroy(s);
}

static void load (softc_entity_t *ptr, const softc_datamodel_t *data_model)
{
  foo_s *self = (foo_s*)ptr;
  size_t new_i_size;
  softc_string_s s;
  softc_datamodel_get_int32(data_model, "n", &self->props.n);
  softc_datamodel_get_string(data_model, "str", &s);
  foo_property_set_str(self, from_softc_string(s));
  softc_datamodel_get_array_double(data_model, "array", &self->props.array, &new_i_size);
}

static const char ** get_dimensions(const softc_entity_t *ptr, size_t *size)
{
  //const foo_s *self = (const foo_s*)ptr;
  return NULL;
}

static int get_dimension_size(const softc_entity_t *ptr, const char *label)
{
  const foo_s *self = (const foo_s*)ptr;
  return self->dims.i_size;
}

static const char * get_meta_type(const softc_entity_t *self)
{
  return FOO_META_TYPE;
}

static const char * get_meta_name(const softc_entity_t *self)
{
  return FOO_META_NAME;
}

static const char * get_meta_namespace(const softc_entity_t *self)
{
  return FOO_META_NAMESPACE;
}

static const char * get_meta_version(const softc_entity_t *self)
{
  return FOO_META_VERSION;
}


foo_s *foo_create(const char *uuid, size_t i_size)
{
  SOFTC_ENTITY_VTABLE(foo);

  foo_s *ret;
  ret = malloc (sizeof *ret);
  *ret = (foo_s){SOFTC_ENTITY_VTABLE_NAME(foo),
		 softc_uuidgen(),
		 (foo_dimensions)
		 {
		   i_size
		 },
		 (foo_properties)
		 {
		   0,
		   calloc(1, sizeof(char)),
		   malloc(i_size * sizeof(double))
		 }
  };
  
  return ret;
}

foo_dimensions * const foo_dims(foo_s *s)
{
  return &s->dims;
}

foo_properties * const foo_props(foo_s* s)
{
  return &s->props;
}

void foo_free(foo_s *foo)
{
  free(foo->props.str);
  free(foo->props.array);
  free((char*)foo->id);
  free(foo);
       
}

void foo_property_set_n (foo_s* self, int value)
{
  self->props.n = value;
}

void foo_property_set_str (foo_s* self, const char *value)
{
  size_t len = strlen(value);
  char* new_str = realloc(self->props.str, 1+len);
  if (new_str) {
    strncpy(new_str, value, 1+len);
    new_str[len] = '\0';
    self->props.str = new_str;
  }
}

void foo_property_set_array (foo_s* self, const double *value, size_t len)
{
  self->props.array = realloc(self->props.array, sizeof (double) * len);
  self->dims.i_size = len;
  memcpy(self->props.array, value, len * sizeof (double));
}

int foo_dimension_get_i (foo_s* self)
{
  return self->dims.i_size;
}

void foo_property_get_n (foo_s* self, int* ret)
{
  *ret = self->props.n;
}

void foo_property_get_str (foo_s* self, char ** ret)
{
  size_t size = strlen (self->props.str);
  (*ret) = realloc((*ret), 1+size * sizeof(char));
  strcpy((*ret), self->props.str);
  strcat((*ret), "\0");
}

void foo_property_get_array (foo_s* self, double **ret, size_t* ret_len)
{
  *ret_len = self->dims.i_size;
  size_t size = self->dims.i_size * sizeof (double);
  (*ret) = malloc(size);
  memcpy((*ret), self->props.array, size);
}

