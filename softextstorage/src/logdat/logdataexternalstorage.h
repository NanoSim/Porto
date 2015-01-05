#ifndef LOGDATAEXTERNALSTORGE_H_DEF
#define LOGDATAEXTERNALSTORGE_H_DEF

#include <iexternalstoragedriver.h>

class LogDataExternalStorage : public IExternalStorageDriver
{
  class LogDataExternalStoragePrivate;
  LogDataExternalStoragePrivate *d;

public:
   LogDataExternalStorage();
   virtual ~LogDataExternalStorage();

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

}; // class LogDataExternalStorage

#endif //  LOGDATAEXTERNALSTORGE_H_DEF
