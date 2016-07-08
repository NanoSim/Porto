#include "softc-entity.h"

struct _softc_entity_t
{
  const struct softc_entity_vtable_ *vtable_;
  const char *id;  
};

void softc_entity_store(const softc_entity_t *self, softc_datamodel_t *model)
{
  self->vtable_->store(self, model);
}

void softc_entity_load(softc_entity_t *self, const softc_datamodel_t *model)
{
  self->vtable_->load(self, model);
}

const char *softc_entity_get_id(const softc_entity_t *self)
{
  return self->id;
}

softc_entity_t *softc_entity_new(const char *uri)
{
  SOFT_UNUSED(uri)
  return NULL;
}

const char * softc_entity_get_meta_type(const softc_entity_t *self)
{
  return self->vtable_->get_meta_type(self);
}

const char * softc_entity_get_meta_name(const softc_entity_t *self)
{
  return self->vtable_->get_meta_name(self);
}

const char * softc_entity_get_meta_namespace(const softc_entity_t *self)
{
  return self->vtable_->get_meta_namespace(self);
}

const char * softc_entity_get_meta_version(const softc_entity_t *self)
{
  return self->vtable_->get_meta_version(self);
}

const char **softc_entity_get_dimensions(const softc_entity_t *self, size_t *size)
{
  return self->vtable_->get_dimensions(self, size);
}

int softc_entity_get_dimension_size (const softc_entity_t *self, const char *label)
{
  return self->vtable_->get_dimension_size(self, label);
}
