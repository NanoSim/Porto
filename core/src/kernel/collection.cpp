#include <list>
#include <map>
#include "idatamodel.h"
#include "collection.h"
#include "tripletstore.h"
#include <assert.h>

SOFT_BEGIN_NAMESPACE

#define NOT_IMPLEMENTED throw std::runtime_error("Not implemented");

/*!
  \todo
  How do we check for correctness after a load?
  How do we specify the Collection schema? We have a name/version field, but
  is this suficient? Probably not!
  Dimension maps using triples should be implemented and tested

*/

class Collection :: Private
{
  friend class Collection;
  std::string name;
  std::string version;
  std::string ns;
  std::map<std::string, IEntity*> entityMap;

  TripletStore tripletStore;
};

/*!
  Constructs a new collection (A new \em uuid will generated and assigned)
 */
Collection :: Collection()
  : IEntity()
  , d (new Collection::Private())
{}

/*!
  Constructs a collection with a given \a uuid
 */
Collection :: Collection(std::string const &uuid)
  : IEntity(uuid)
  , d(new Collection::Private())
{
}

/*!
  Constructs a collection from another \a entity
  \sa IEntity
 */
Collection :: Collection(const IEntity *entity)
  : IEntity(entity)
  , d (new Collection::Private())
{
}

/*!
  Constructs a collection from a data model \a dm
*/
Collection :: Collection(IDataModel const *dm)
  : IEntity()
  , d(new Collection::Private())
{
  load(dm);
}

/*!
  Destroys the collection
 */
Collection :: ~Collection()
{
  delete d;
}

/*!
  Creates a new Collection with a given \a uuid
*/
IEntity *Collection :: create (std::string const &uuid)
{
  if (uuid.empty()) {
    return dynamic_cast<IEntity*>(new Collection());
  }
  Collection *collection = new Collection(uuid);
  return dynamic_cast<IEntity*>(collection);
}

/*!
  Get the name of the collection
 */
std::string Collection :: name() const
{
  return d->name;
}

/*!
  Sets the \a name of the collection
 */
void Collection :: setName(std::string const &name)
{
  d->name = name;
}

/*!
  Get the version of the collection
*/
std::string Collection :: version() const
{
  return d->version;
}

/*!
  Set the \a version of the collection
*/
void Collection :: setVersion(std::string const &version)
{
  d->version = version;
}

/*!
  Get the namespace of the collection
 */
std::string Collection :: ns() const
{
  return d->ns;
}

/*!
  Sets the \a namespace of the collection
 */
void Collection :: setNamespace(std::string const &ns)
{
  d->ns = ns;
}

/*!
  Register an entity with a given \a label.
  \a label is the internal name of the entity instance
  \a entity a reference to an IEntity

  \sa IEntity
*/
void Collection :: registerEntity(std::string const &label, IEntity const *entity)
{
  this->addEntity(label,
          entity->metaName(),
          entity->metaVersion(),
          entity->metaNamespace(),
          entity->id());
}

/*!
  Add entity information to the collection using
  \a label is the internal name of the entity instance
  \a name the formal entity meta-name
  \a version the formal entity meta-version
  \a ns the formal entity meta-namespace
  \a uuid the identity of the entity
  \todo: Remove one of these duplicates?
*/
void Collection :: addEntity(std::string const &label,
         std::string const &name,
         std::string const &version,
         std::string const &ns,
         std::string const &uuid)
{
  addRelation(label, "is-a", "Entity");
  addRelation(label, "name", name);
  addRelation(label, "version", version);
  addRelation(label, "namespace", ns);
  addRelation(label, "id", uuid);
}

/*!
  Find objects where the subject has a give predicate. Note that
  reverse predicates can be employed by using the prefix ^
 */
std::list<std::string> Collection :: findRelations(std::string const &subject,
                                                  std::string const &pred)
{
  return d->tripletStore.findTriplets(subject, pred);
}

/*!
  Find the entity given the label \a label
  \a name Reference the meta name
  \a version Reference the meta version
  \a ns Reference the meta namespace
  \a uuid Reference to the entity identity

  \todo error handling is not adequate
 */
void Collection :: findEntity(std::string const &label,
			      std::string &name,
			      std::string &version,
			      std::string &ns,
			      std::string &uuid) const
{
  auto findFirstTriplet = [this, label](std::string const &pred) -> std::string {
    auto t = this->d->tripletStore.findTriplets(label, pred);
    if (t.size() > 0) return t.front();
    return std::string();
  };

  name = findFirstTriplet("name");
  version = findFirstTriplet("version");
  ns = findFirstTriplet("namespace");
  uuid = findFirstTriplet("id");
}

/*!
  Attaches the entity to this collection so that it is loaded and stored
  with the collection.
*/
void Collection :: attachEntity(std::string const &label, IEntity *entity) {
  registerEntity(label, entity);
  d->entityMap.insert({label, entity});
}

/*!
  Returns the number of registered entities
 */
int Collection :: numEntities() const
{
  return d->tripletStore.findTriplets("Entity", "^is-a").size();
}

/*!
  Add a dimensionality \a label with an optional \a description
 */
void Collection :: addDim(std::string const &label,
              std::string const &description)
{
  NOT_IMPLEMENTED
}

void Collection :: addDimMap(std::string const &label,
                             std::string const &entityDim,
                             std::string const &collectionDim)
{
  NOT_IMPLEMENTED
}


void Collection :: addRelation(std::string const &subject,
                   std::string const &predicate,
                   std::string const &object)
{
  d->tripletStore.addTriplet(subject, predicate, object);
}

// TODO: Remove this duplicate here, replaced by addRelation
void Collection :: connect (std::string const &subject,
                std::string const &predicate,
                std::string const &object)
{
  addRelation(subject, predicate, object);
}

int Collection :: numRelations() const
{
  return d->tripletStore.size();
}

/*!
  Restore state from a given \dataModel

  \sa IDataModel
 */
void Collection :: load (IDataModel const *dataModel)
{
  setId(dataModel->id());

  dataModel->getString("__name__", d->name);
  dataModel->getString("__version__", d->version);

  std::string csv;
  dataModel->getString("triplets", csv);
  d->tripletStore.fromCSV(csv);

  // Also attach entities that we have data models for
  auto labels = d->tripletStore.findTriplets("Entity", "^is-a");
  for (auto l: labels) {
    IDataModel *dm = dataModel->getModel(l.c_str());
    if (dm) {
      // Pass the datamodel on to attached entities
      auto ie = d->entityMap.find(l);
      if (ie != d->entityMap.end()) {
        IEntity *e = ie->second;
        dm->setMetaName(e->metaName());
        dm->setMetaVersion(e->metaVersion());
        dm->setMetaNamespace(e->metaNamespace());
        e->load(dm);
        e->setId(dm->id());
      }
    }
  }
}

/*!
  Store the collection state to a given \dataModel

  \sa IDataModel
 */
void Collection :: save (IDataModel *dataModel) const
{
  dataModel->setId(id());

  dataModel->appendString("__name__", d->name);
  dataModel->appendString("__version__", d->version);

  dataModel->appendString("triplets", d->tripletStore.toCSV());

  // Also perform a save on all attached entities.
  for(auto &ie: d->entityMap) {
    // Creates an empty clone of the same data model type
    // TODO: Who owns this data model now? Needs to be freed at some point?
    auto dm = dataModel->createModel();
    IEntity *e = ie.second;
    // TODO: Note that this part may crash if the datamodel is free'd
    // (for example by using a smart pointer) before passing it on
    // here. This indicates that there is an ownership issue here that
    // we MUST resolve.
    dm->setId(e->id());
    dm->setMetaName(e->metaName());
    dm->setMetaVersion(e->metaVersion());
    dm->setMetaNamespace(e->metaNamespace());

    e->save(dm);
    dataModel->appendModel(ie.first.c_str(), dm);
  }
}

/*!
  Returns an attached instance with given \a label.
  If the label is not found the functino will return a nullptr.
 */
IEntity const *Collection :: findInstance(std::string const &label) const
{
  const auto it = d->entityMap.find(label);
  if (it != d->entityMap.cend()) {
    return (*it).second;
  }
  return nullptr;
}

std::vector<std::string> Collection :: dimensions() const {
  NOT_IMPLEMENTED
}

int Collection :: getDimensionSize(std::string const &dim) const
{
  NOT_IMPLEMENTED
}

SOFT_END_NAMESPACE
