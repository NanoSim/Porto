/* -*- C -*-  (not really, but good for syntax highlighting) */

/*
 * C code
 */
%{

/* SWIG needs a definition of softc_collection_s in order to expose
 * the type as well as for memory management.  Even an inaccurate one
 * will do... */
struct softc_collection_s {
  void *_unused;
  char *id;
};

#include "softc-collection.h"

const char *softc_collection_get_id(const softc_collection_s *self)
{
  return softc_entity_get_id((const softc_entity_t *)self);
}

const char *softc_collection_get_meta_name(const softc_collection_s *self)
{
  return softc_entity_get_meta_name((const softc_entity_t *)self);
}

const char *softc_collection_get_meta_version(const softc_collection_s *self)
{
  return softc_entity_get_meta_version((const softc_entity_t *)self);
}

const char *softc_collection_get_meta_namespace(const softc_collection_s *self)
{
  return softc_entity_get_meta_namespace((const softc_entity_t *)self);
}


void collection_get_name(softc_collection_s *collection, char **name)
{
  softc_string_s s = softc_string_create("");
  softc_collection_get_name(collection, s);
  *name = strdup(from_softc_string(s));
  softc_string_destroy(s);
}

void collection_get_version(softc_collection_s *collection,
                                   char **version)
{
  softc_string_s s = softc_string_create("");
  softc_collection_get_version(collection, s);
  *version = strdup(from_softc_string(s));
  softc_string_destroy(s);
}

void collection_get_namespace(softc_collection_s *collection,
                                     char **namespace)
{
  softc_string_s s = softc_string_create("");
  softc_collection_get_namespace(collection, s);
  *namespace = strdup(from_softc_string(s));
  softc_string_destroy(s);
}

void collection_register_collection(softc_collection_s *collection,
                                          const char *label,
                                          softc_collection_s *subcoll)
{
  softc_collection_register_entity(collection, label,
                                   (softc_entity_t *)subcoll);
}

void collection_load(softc_storage_t *storage, softc_collection_s *collection)
{
  softc_storage_load(storage, (softc_entity_t*)collection);
}
%}


/*
 * Typemaps
 */
%typemap(in,numinputs=0) char **OUTSTR (char *temp) {
  $1 = &temp;
}
%typemap(argout) char **OUTSTR {
  $result = string_FromString(*$1);
}


/*
 * SWIG declarations
 */

/* Expose our dummy definition of softc_collection_s. */
struct softc_collection_s {
  void *_unused;
  char *id;
};

%newobject softc_collection_create_new;
%newobject softc_collection_create;
%delobject softc_collection_free;


softc_collection_s *softc_collection_create_new();
softc_collection_s *softc_collection_create(const char *id);
//softc_collection_s *softc_collection_create_ext(const char *name,
//						const char *version);
void softc_collection_free(softc_collection_s *collection);

const char *softc_collection_get_id(const softc_collection_s *self);
const char *softc_collection_get_meta_name(const softc_collection_s *self);
const char *softc_collection_get_meta_version(const softc_collection_s *self);
const char *softc_collection_get_meta_namespace(const softc_collection_s *self);

//softc_collection_properties_s * const softc_collection_props(softc_collection_s *collection);

//softc_collection_dimensions_s * const softc_collection_dims(softc_collection_s *collection);

//softc_collection_allocatable_s * softc_collection_allocatables(softc_collection_s *collection);

void softc_collection_register_entity(softc_collection_s *collection,
				      const char *label,
				      softc_entity_t *entity);

void collection_register_collection(softc_collection_s *collection,
                                    const char *label,
                                    softc_collection_s *subcoll);

size_t softc_collection_num_entities(softc_collection_s *collection);

size_t softc_collection_num_relations(softc_collection_s *collection);

softc_string_list_s *softc_collection_find_relations(softc_collection_s *collection,
						     const char *subject,
						     const char *predicate);

void collection_get_name(softc_collection_s *collection,
				char **OUTSTR);

void softc_collection_set_name(softc_collection_s *collection,
			       const char *name);

void collection_get_version(softc_collection_s *collection,
                                   char **OUTSTR);

void softc_collection_set_version(softc_collection_s *collection,
				  const char *version);

void collection_get_namespace(softc_collection_s *collection,
                                     char **OUTSTR);

void softc_collection_set_namespace(softc_collection_s *collection,
			       const char *namespace);

void collection_load(softc_storage_t *storage,
                     softc_collection_s *collection);

void softc_collection_add_relation(softc_collection_s *collection,
				   const char *subject,
				   const char *predicate,
				   const char *object);

void softc_collection_connect(softc_collection_s *collection,
			      const char *subject,
			      const char *predicate,
			      const char *object);
