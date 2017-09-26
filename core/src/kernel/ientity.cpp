#include <QUuid>
#include <QString>

#include "ientity.h"
SOFT_BEGIN_NAMESPACE

class IEntity::Private
{
   friend class IEntity;
  Private(std::string const &id)
    : uuid(id.c_str())
  {}

  Private()
    : uuid(QUuid::createUuid())
  {}

  QUuid uuid;
};

/*!
  Constructs a new entity (A new \em uuid will generated and assigned)
 */
IEntity :: IEntity()
   :d (new IEntity::Private())
{}

/*!
  Constructs an entity with a given \a uuid
 */
IEntity :: IEntity(std::string const &uuid)
   : d(new IEntity::Private(uuid))
{}

/*!
  Construct entity from another entity
 */
IEntity :: IEntity(IEntity const *other)
  : d(other != nullptr ? new IEntity::Private(other->id()) : new IEntity::Private())
{}

/*!
  Destroy the entity
 */
IEntity :: ~IEntity()
{
   delete d;
}

/*!  
 */
IEntity* create (const std::string &uuid)
{
  return nullptr;
}

/*!
  Get the entity unique identitier
 */
std::string IEntity :: id() const
{
  // QUuid will return curly the UUID with curly braces,
  // we strip these off when returning the uuid-string
  return d->uuid.toString().mid(1,36).toStdString();
}

/*!
  Set the entity unique identitier
  \warning Do not use this unless you really know what you are doing!!!
 */

void IEntity :: setId(const std::string &id) {
  d->uuid = QUuid(id.c_str());
}

SOFT_END_NAMESPACE
