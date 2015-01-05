#ifndef IENTITY_H_DEF
#define IENTITY_H_DEF

#include "transactor.h"

class StorageObject;
class IEntity 
{
   friend class Transactor;

public:
   virtual ~IEntity() = 0;

protected:
   virtual bool store(StorageObject &object) = 0;
   virtual bool restore(StorageObject const &storageObject) = 0;
};

#endif //IENTITY_H_DEF
