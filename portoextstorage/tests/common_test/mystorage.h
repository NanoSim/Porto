#ifndef MYSTORAGE_H_DEF
#define MYSTORAGE_H_DEF
#include <iexternalstoragedriver.h>

const std::string NAME("mystorage");
const std::string VERSION("0.0.1");

class MyStorage : public IExternalStorageDriver
{
public:
   MyStorage(){}
   virtual ~MyStorage(){}
 
   bool open(char const *url);   
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
};

bool MyStorage :: open(char const *url) 
{
   return false;
}

inline std::string MyStorage :: name() const
{
   return std::string(NAME);
}

inline std::string MyStorage :: version() const
{
   return std::string(VERSION);
}

std::string MyStorage :: readString(char const* propertyName) const
{
   return std::string();
}

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
