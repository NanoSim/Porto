#include <list>
#include "transactor.h"
#include "storage.h"
#include "ientity.h"
#include "storageobject.h"

class Transactor::Private
{
   friend class Transactor;
   
   Private(Storage const &storage)
      : storage(storage)
   {}

   Storage const storage;
   std::list<IEntity*> entityList;
   std::list<StorageObject> storageObjectList;
};

Transactor :: Transactor(Storage const &storage)
   : d(new Private (storage))
{}

Transactor :: ~Transactor()
{
   delete d;
}

Transactor Transactor :: create(Storage const &storage)
{
   Transactor tr(storage);
   return tr;
}

void Transactor :: store()
{
   d->storageObjectList.clear();
   for(auto entity : d->entityList) {
      StorageObject storageObject;
      entity->store(storageObject);
      d->storageObjectList.push_back(storageObject);
   }
}

void Transactor :: fetch()
{
   auto it = d->storageObjectList.begin();
   for(auto entity : d->entityList) {
      entity->restore(*it);
      it++;
   }
}

void Transactor :: add(IEntity *entity)
{
   d->entityList.push_back(entity);
}
