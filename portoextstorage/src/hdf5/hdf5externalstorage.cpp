#include "hdf5reader.h"
#include "hdf5externalstorage.h"

std::string const drivername("HDF5");
std::string const driverversion("1.0");

class HDF5ExternalStorage :: HDF5ExternalStoragePrivate
{
   friend class HDF5ExternalStorage;
};

HDF5ExternalStorage :: HDF5ExternalStorage()
   : d(new HDF5ExternalStoragePrivate())
{}

HDF5ExternalStorage :: ~HDF5ExternalStorage()
{
   delete d;
}

bool HDF5ExternalStorage :: open(char const* url)
{
   return true;
}

std::string HDF5ExternalStorage :: readString(char const* propertyName) const
{
   return std::string();
}

std::string HDF5ExternalStorage :: name() const
{
   return drivername;
}

std::string HDF5ExternalStorage :: version() const
{
   return driverversion;
}


bool HDF5ExternalStorage :: readDouble(char const* propertyName, double *target)
{
   return true;
}

bool HDF5ExternalStorage :: readInt(char const* propertyName, int *target)
{
   return true;
}

bool HDF5ExternalStorage :: readDoubleArray(char const* propertyName, int const rank, int const *dimensions, double *target)
{
   return true;
}

bool HDF5ExternalStorage :: readIntArray(char const* propertyName, int const rank, int const *dimensions, int *target)
{
   return true;
}

int HDF5ExternalStorage :: getRank(char const* propertyName)
{
   return 0;
}

bool HDF5ExternalStorage :: getDimensions(char const* propertyName, int *target)
{
   return true;
}

int HDF5ExternalStorage :: getNumProperties()
{
   return 0;
}

bool HDF5ExternalStorage :: getPropertyName(int index, char *target)
{
   return true;
}

