#include <list>
#include <QDebug>

#include "qbson.h"
#include "entity.h"
#include "transactor.h"

class Transactor::Private
{
  friend class Transactor;
  
  std::list<Entity*> entityList;
};

Transactor :: Transactor()
  : d (new Transactor::Private)
{}

Transactor :: ~Transactor()  
{
  delete d;
}

bool Transactor :: save()
{  
  for (auto e: d->entityList) {
    soft::QBson bson;
    e->store(bson);
    qDebug() << bson.asJson();
  };
  return true;
}

Transactor &Transactor :: operator<<(Entity &e)
{
  d->entityList.push_back(&e);
  return *this;
}
