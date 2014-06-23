#include "storage.h"
#include "unit.h"
#include "example.h"
#include <iostream>

using std::cout;
using std::endl;

Transactor setupStorage()
{
   Storage storage = StorageCreator::connect("mongodb")
      .addOption("database", "porto")
      .addOption("collection", "mystorage");

   return Transactor::create(storage);
}

int main(int, char**)
{
   auto transactor = setupStorage();

   Unit unit;
   cout << unit._name() << " " << unit._version() << endl;

   unit.abbreviation         = "Gy/s";
   unit.code                 = "0780";
   unit.conversionMultiplier = 1.0e0;
   unit.conversionOffset     = 0.0;
   unit.quantityKind         = "quantity:AbsorbedDoseRate";
   unit.symbol               = "Gy/s";
   unit.unit                 = "GrayPerSecond";

   transactor.add(&unit);
   transactor.store();
   
   /* Set new value for unit.code */
   unit.code = "3421";
   cout << unit.code << endl;

   /* Restore state from transactor */
   transactor.fetch();
   cout << unit.code << endl;

   return EXIT_SUCCESS;
}
