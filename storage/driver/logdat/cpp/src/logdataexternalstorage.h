#ifndef LOGDATAEXTERNALSTORGE_H_DEF
#define LOGDATAEXTERNALSTORGE_H_DEF

#include <iexternalstorage.h>

class LogDataExternalStorage : public IExternalStorage
{
  class LogDataExternalStoragePrivate;
  LogDataExternalStoragePrivate *d;

public:
   LogDataExternalStorage();
   virtual ~LogDataExternalStorage();

   bool readDouble(char const* propertyName, double *target);
   bool readInt(char const* propertyName, int *target);
   bool readDoubleArray(char const* propertyName, int const rank, int const *dimensions, double *target);
   bool readIntArray(char const* propertyName, int const rank, int const *dimensions, int *target);
   int getRank(char const* propertyName);
   bool getDimensions(char const* propertyName, int *target);
   int getNumProperties();
   bool getPropertyName(int index, char *target);

}; // class LogDataExternalStorage

#endif //  LOGDATAEXTERNALSTORGE_H_DEF
