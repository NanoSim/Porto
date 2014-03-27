#include <QtGlobal>
#include <QCoreApplication>
#include <QVariant>
#include <QtDebug> 

#include "externalstorage.h"

class ExternalStorage :: ExternalStoragePrivate
{
   friend class ExternalStorage;

   ExternalStoragePrivate(IExternalStorage *storage)
      : storage(storage)
   {}

   ~ExternalStoragePrivate()
   {
      storage = 0;
   }

   IExternalStorage *storage;
};

ExternalStorage :: ExternalStorage(IExternalStorage *istorage)
   : d(new ExternalStoragePrivate(istorage))
{}

ExternalStorage :: ExternalStorage()
   : d(0)
{}

ExternalStorage :: ~ExternalStorage()
{}

void ExternalStorage :: registerExternalStorageDriver(char const *name, IExternalStorage *istorage)
{
   Q_CHECK_PTR(qApp);
   if (qApp->property(name).isValid()) {
      qWarning() << name << "already registered.";
      return;
   }
   qApp->setProperty(qPrintable(name), qVariantFromValue((void*) istorage));
   Q_ASSERT(qApp->property(name).isValid());
}


ExternalStorage ExternalStorage :: addExternalStorageDriver(char const *name)
{
   QVariant v = qApp->property(name);

   if(v.isValid()) {
      IExternalStorage *storage = (IExternalStorage*)v.value<void *>();
      return ExternalStorage(storage);
   }	 
   
   qWarning() << "Unregistered external storage driver: " << name;
   return ExternalStorage();
}

IExternalStorage *ExternalStorage :: driver()
{
   if(!d)
      return 0;

   return d->storage;
}
