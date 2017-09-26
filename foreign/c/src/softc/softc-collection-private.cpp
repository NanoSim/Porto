#include "softc-collection-private.h"
#include <cassert>
#include <collection.h>
#include <memory>
#include <map>

#include "softc-entity.h"
#include "softc-datamodel-private.hpp"

struct softc_private_s {
  std::unique_ptr<soft::Collection> collection;
  std::map<std::string, softc_entity_t*> entityMap;
};

void *softc_collection_private_create(const char *id)
{
  softc_private_s *d = new softc_private_s();
  assert( d != nullptr);
  d->collection = std::unique_ptr<soft::Collection>(new soft::Collection(id));
  return (void*)d;
}

void softc_collection_private_register_entity(void *ref, const char *label, softc_entity_t *entity)
{
  softc_private_s *d = static_cast<softc_private_s*>(ref);
  assert(d != nullptr);

  d->entityMap.insert({std::string(label), entity});
  d->collection->addEntity(label,
			   softc_entity_get_meta_name(entity),
			   softc_entity_get_meta_version(entity),
			   softc_entity_get_meta_namespace(entity),
			   softc_entity_get_id(entity));
}

void softc_collection_private_find_entity( void *ref,
                                           const char *label,
                                           softc_string_s name,
                                           softc_string_s version,
                                           softc_string_s ns,
                                           softc_string_s uuid)
{
  softc_private_s *d = static_cast<softc_private_s*>(ref);
  assert(d != nullptr);
  std::string iname;
  std::string iversion;
  std::string ins;
  std::string iuuid;

  d->collection->findEntity(label, iname, iversion, ins, iuuid);

  softc_string_assign(name   ,  iname.c_str()   );
  softc_string_assign(version,  iversion.c_str());
  softc_string_assign(ns     ,  ins.c_str()     );
  softc_string_assign(uuid   ,  iuuid.c_str()   );
}

void softc_collection_private_add_relation( void *ref,
                                            const char *subject,
                                            const char *predicate,
                                            const char *object)
{
  softc_private_s *d = static_cast<softc_private_s*>(ref);
  assert(d != nullptr);
  d->collection->addRelation(subject, predicate, object);
}

void softc_collection_private_connect(void *ref, const char *subject, const char *predicate, const char *object)
{
  softc_private_s *d = static_cast<softc_private_s*>(ref);
  assert(d != nullptr);
  d->collection->connect(subject, predicate, object);
}

void softc_collection_private_get_name(void *ref, softc_string_s name)
{
  softc_private_s *d = static_cast<softc_private_s*>(ref);
  assert(d != nullptr);
  auto n = d->collection->name();
  softc_string_assign(name, n.c_str());
}

void softc_collection_private_get_version(void *ref, softc_string_s version)
{
  softc_private_s *d = static_cast<softc_private_s*>(ref);
  assert(d != nullptr);
  auto n = d->collection->version();
  softc_string_assign(version, n.c_str());
}

void softc_collection_private_get_namespace(void *ref, softc_string_s ns)
{
  softc_private_s *d = static_cast<softc_private_s*>(ref);
  assert(d != nullptr);
  auto n = d->collection->ns();
  softc_string_assign(ns, n.c_str());
}

void softc_collection_private_set_name(void *ref, const char *name)
{
  softc_private_s *d = static_cast<softc_private_s*>(ref);
  assert(d != nullptr);
  d->collection->setName(name);
}

void softc_collection_private_set_version(void *ref, const char *version)
{
  softc_private_s *d = static_cast<softc_private_s*>(ref);
  assert(d != nullptr);
  d->collection->setVersion(version);
}

void softc_collection_private_set_namespace(void *ref, const char *ns)
{
  softc_private_s *d = static_cast<softc_private_s*>(ref);
  assert(d != nullptr);
  d->collection->setNamespace(ns);
}

int softc_collection_private_num_relations(void *ref)
{
  softc_private_s *d = static_cast<softc_private_s*>(ref);
  assert(d != nullptr);
  return d->collection->numRelations();
}

int softc_collection_private_num_entities(void *ref)
{
  softc_private_s *d = static_cast<softc_private_s*>(ref);
  assert(d != nullptr);
  return d->collection->numEntities();
}

void softc_collection_private_save(const void *ref, softc_datamodel_t *dataModel)
{
  const softc_private_s *d = static_cast<const softc_private_s*>(ref);
  assert(d != nullptr);

  d->collection->save(dataModel->ref);
}

void softc_collection_private_load(void *ref, const softc_datamodel_t *dataModel)
{
  const softc_private_s *d = static_cast<softc_private_s*>(ref);
  assert(d != nullptr);

  d->collection->load(dataModel->ref);
}

softc_string_list_s *softc_collection_private_find_relations(void *ref, const char *subject, const char *predicate)
{
  softc_private_s *d = static_cast<softc_private_s*>(ref);
  assert(d != nullptr);
  auto rs = d->collection->findRelations(subject, predicate);
  auto n = rs.size();

  auto lst = softc_string_list_create();
  for (auto relation : rs) {
    auto str = softc_string_create(relation.c_str());
    softc_string_list_append(lst, str);
  }
  return lst;
}
