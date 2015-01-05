#ifndef UNIT_H_DEF
#define UNIT_H_DEF

#include "ientity.h"
#include <string>

class Unit : public IEntity
{
public:
   Unit();
   Unit(Unit const &other);
   virtual ~Unit();
   
   std::string abbreviation;
   std::string code;
   double conversionMultiplier;
   double conversionOffset;
   std::string quantityKind;
   std::string symbol;
   std::string unit;

protected:
   virtual bool store(StorageObject &storageObject);
   virtual bool restore(StorageObject const &storageObject);
};

class UnitStore
{
public:
   static bool set(Unit const *unit, StorageObject &obj);
   static bool get(Unit *unit, StorageObject const &obj);
};

#endif // UNIT_H_DEF
