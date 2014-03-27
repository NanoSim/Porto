#ifndef EXTERNALSTORAGE_H_DEF
#define EXTERNALSTORAGE_H_DEF

#include "iexternalstoragedriver.h"

class ExternalStorage
{
private:
   ExternalStorage(IExternalStorageDriver *istorage);

public:
   ExternalStorage();
   virtual ~ExternalStorage();

   static void registerExternalStorageDriver(char const *name, IExternalStorageDriver *istorage);
   static ExternalStorage addExternalStorageDriver(char const *name);

   IExternalStorageDriver *driver();

private:
   class ExternalStoragePrivate;
   ExternalStoragePrivate *d;
}; // class ExternalStorage

#endif // EXTERNALSTORAGE_H_DEF

