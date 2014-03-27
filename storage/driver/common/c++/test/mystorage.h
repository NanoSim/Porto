#ifndef MYSTORAGE_H_DEF
#define MYSTORAGE_H_DEF
#include <iexternalstorage.h>

class MyStorage : public IExternalStorage
{
public:
   MyStorage(){}
   virtual ~MyStorage(){}

   bool readDouble(char const* propertyName, double *target);
   bool readInt(char const* propertyName, int *target);
   bool readDoubleArray(char const* propertyName, int const rank, int const *dimensions, double *target);
   bool readIntArray(char const* propertyName, int const rank, int const *dimensions, int *target);
   int getRank(char const* propertyName);
   bool getDimensions(char const* propertyName, int *target);
   int getNumProperties();
   bool getPropertyName(int index, char *target);
};

inline bool MyStorage :: readDouble(char const* propertyName, double *target)
{
   *target = 3.14;
   return true;
}

inline bool MyStorage :: readInt(char const* propertyName, int *target)
{
   *target = 42;
   return true;
}

inline bool MyStorage :: readDoubleArray(char const* propertyName, int const rank, int const *dimensions, double *target)
{
   return true;
}

inline bool MyStorage :: readIntArray(char const* propertyName, int const rank, int const *dimensions, int *target)
{
   return true;
}

inline int MyStorage :: getRank(char const* propertyName)
{
   return 0;
}

inline bool MyStorage :: getDimensions(char const* propertyName, int *target)
{
   return true;
}

inline int MyStorage :: getNumProperties()
{
   return 0;
}

inline bool MyStorage :: getPropertyName(int index, char *target)
{
   return true;
}

#endif
