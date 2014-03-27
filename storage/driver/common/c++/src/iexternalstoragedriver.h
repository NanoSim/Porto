#ifndef IEXTERNALSTORAGEDRIVER_H_DEF
#define IEXTERNALSTORAGEDRIVER_H_DEF

class IExternalStorageDriver
{
public:
   IExternalStorageDriver();
   virtual ~IExternalStorageDriver() = 0; 

   virtual bool readDouble(char const* propertyName, double *target) = 0;
   virtual bool readInt(char const* propertyName, int *target) = 0;
   virtual bool readDoubleArray(char const* propertyName, int const rank, int const *dimensions, double *target) = 0;
   virtual bool readIntArray(char const* propertyName, int const rank, int const *dimensions, int *target) = 0;
   virtual int getRank(char const* propertyName) = 0;
   virtual bool getDimensions(char const* propertyName, int *target) = 0;
   virtual int getNumProperties() = 0;
   virtual bool getPropertyName(int index, char *target) = 0;
}; // class IExternalStorageDriver


#endif // IEXTERNALSTORAGEDRIVER_H_DEF
