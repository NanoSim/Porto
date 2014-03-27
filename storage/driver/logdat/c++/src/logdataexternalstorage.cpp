#include "logdatareader.h"
#include "logdataexternalstorage.h"

class LogDataExternalStorage :: LogDataExternalStoragePrivate
{
   friend class LogDataExternalStorage;
};

LogDataExternalStorage :: LogDataExternalStorage()
   : d(new LogDataExternalStoragePrivate())
{}

LogDataExternalStorage :: ~LogDataExternalStorage()
{
   delete d;
}

bool LogDataExternalStorage :: readDouble(char const* propertyName, double *target)
{
   return true;
}

bool LogDataExternalStorage :: readInt(char const* propertyName, int *target)
{
   return true;
}

bool LogDataExternalStorage :: readDoubleArray(char const* propertyName, int const rank, int const *dimensions, double *target)
{
   return true;
}

bool LogDataExternalStorage :: readIntArray(char const* propertyName, int const rank, int const *dimensions, int *target)
{
   return true;
}

int LogDataExternalStorage :: getRank(char const* propertyName)
{
   return 0;
}

bool LogDataExternalStorage :: getDimensions(char const* propertyName, int *target)
{
   return true;
}

int LogDataExternalStorage :: getNumProperties()
{
   return 0;
}

bool LogDataExternalStorage :: getPropertyName(int index, char *target)
{
   return true;
}

