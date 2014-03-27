#ifndef EXTERNALSTORAGE_H_DEF
#define EXTERNALSTORAGE_H_DEF

#include "iexternalstorage.h"

class ExternalStorage
{
private:
   ExternalStorage(IExternalStorage *istorage);

public:
   ExternalStorage();
   virtual ~ExternalStorage();

   static void registerExternalStorageDriver(char const *name, IExternalStorage *istorage);
   static ExternalStorage addExternalStorageDriver(char const *name);

   IExternalStorage *driver();

private:
   class ExternalStoragePrivate;
   ExternalStoragePrivate *d;
}; // class ExternalStorage

#endif // EXTERNALSTORAGE_H_DEF

