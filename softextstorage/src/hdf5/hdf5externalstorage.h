#ifndef HDF5EXTERNALSTORGE_H_DEF
#define HDF5EXTERNALSTORGE_H_DEF

#include <iexternalstoragedriver.h>

class HDF5ExternalStorage : public IExternalStorageDriver
{
  class HDF5ExternalStoragePrivate;
  HDF5ExternalStoragePrivate *d;

public:
   HDF5ExternalStorage();
   virtual ~HDF5ExternalStorage();

   bool open(char const* url);
   std::string name() const;
   std::string version() const;

   std::string readString(char const* propertyName) const;
   bool readDouble(char const* propertyName, double *target);
   bool readInt(char const* propertyName, int *target);
   bool readDoubleArray(char const* propertyName, int const rank, int const *dimensions, double *target);
   bool readIntArray(char const* propertyName, int const rank, int const *dimensions, int *target);
   int getRank(char const* propertyName);
   bool getDimensions(char const* propertyName, int *target);
   int getNumProperties();
   bool getPropertyName(int index, char *target);

}; // class HDF5ExternalStorage

#endif //  HDF5EXTERNALSTORGE_H_DEF
