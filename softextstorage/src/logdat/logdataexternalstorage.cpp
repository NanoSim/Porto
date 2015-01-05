#include "logdatareader.h"
#include "logdataexternalstorage.h"

std::string const drivername("LogData");
std::string const driverversion("1.0");

class LogDataExternalStorage :: LogDataExternalStoragePrivate
{
   friend class LogDataExternalStorage;

   LogDataExternalStoragePrivate()
      : reader(0)
   {}

   ~LogDataExternalStoragePrivate()
   {
      delete reader;
   }

   LogDataReader *reader;
   LogData data;
};

LogDataExternalStorage :: LogDataExternalStorage()
   : d(new LogDataExternalStoragePrivate())
{}

LogDataExternalStorage :: ~LogDataExternalStorage()
{
   delete d;
}

bool LogDataExternalStorage :: open(char const* url)
{
   if( d->reader != 0 ) delete d->reader;
   d->reader = new LogDataReader(QString(url));
   Q_CHECK_PTR(d->reader);

   d->data    = d->reader->readLogData();
   return true;
}

std::string LogDataExternalStorage :: readString(char const* propertyName) const
{
   std::string name(propertyName);
   if( name.compare("vectorDataName") )
      return std::string(qPrintable(d->data.vectorDataName));
   if( name.compare("scalarDataName") )
      return std::string(qPrintable(d->data.scalarDataName));

   return std::string();
}


std::string LogDataExternalStorage :: name() const
{
   return drivername;
}

std::string LogDataExternalStorage :: version() const
{
   return driverversion;
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

