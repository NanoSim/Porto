#include "unit.h"
#include "storageobject.h"
#include "storagevalue.h"
#include <iostream>
#include <string>

Unit :: Unit()
{}

Unit :: Unit(Unit const &other)
   : abbreviation (other.abbreviation)
   , code (other.code)
   , conversionMultiplier (other.conversionMultiplier)
   , quantityKind (other.quantityKind)
   , symbol (other.symbol)
   , unit (other.unit)
{}

Unit :: ~Unit()
{}

bool Unit :: store(StorageObject &obj)
{
   return UnitStore::set(this, obj);
}

bool Unit :: restore(StorageObject const &obj)
{
   return UnitStore::get(this, obj);
}

bool UnitStore :: set(const Unit *unit, StorageObject &obj)
{
   obj.insert("abbreviation",         StorageValue(unit->abbreviation));
   obj.insert("code",                 StorageValue(unit->code));
   obj.insert("conversionMultiplier", StorageValue(unit->conversionMultiplier));
   obj.insert("conversionOffset",     StorageValue(unit->conversionOffset));
   obj.insert("quantityKind",         StorageValue(unit->quantityKind));
   obj.insert("symbol",               StorageValue(unit->symbol));
   obj.insert("unit",                 StorageValue(unit->unit));
   return true;
}

bool UnitStore :: get(Unit *unit, StorageObject const &obj)
{
   unit->abbreviation         = obj.find("abbreviation").toString();
   unit->code                 = obj.find("code").toString();
   unit->conversionMultiplier = obj.find("conversionMultiplier").toDouble();
   unit->conversionOffset     = obj.find("conversionOffset").toDouble();
   unit->quantityKind         = obj.find("quantityKind").toString();
   unit->symbol               = obj.find("symbol").toString();
   unit->unit                 = obj.find("unit").toString();
   return true;
}
