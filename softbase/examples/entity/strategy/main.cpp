#include <iostream>

#include "transactor.h"
#include "storage.h"
#include "unit.h"
#include "storagevalue.h"

int main (int, char *argv[])
{
   Storage storage = StorageCreator::connect("mongodb")
      .addOption("database", "soft")
      .addOption("collection", "mystorage");

   auto transactor = Transactor::create(storage);
   std::cout << storage.driver() << std::endl;
   std::cout << storage.option("database") << std::endl;

   Unit unit;
   unit.abbreviation         = "Gy/s";
   unit.code                 = "0780";
   unit.conversionMultiplier = 1.0e0;
   unit.conversionOffset     = 0.0;
   unit.quantityKind         = "quantity:AbsorbedDoseRate";
   unit.symbol               = "Gy/s";
   unit.unit                 = "GrayPerSecond";

   transactor.add(&unit);
   transactor.store();

   unit.code = "2222";
   std::cout << unit.code << std::endl;
   transactor.retrieve();
   std::cout << unit.code << std::endl;

   return EXIT_SUCCESS;
}
