#ifndef ISTORAGEVALUE_H_DEF
#define ISTORAGEVALUE_H_DEF

#include <string>

class IStorageValue
{
public:
   IStorageValue () {};
   virtual ~IStorageValue() = 0;

   virtual bool isInt() const = 0;
   virtual bool isDouble() const = 0;
   virtual bool isString() const = 0;

   virtual int toInt() const = 0;
   virtual double toDouble() const = 0;
   virtual std::string toString() const = 0;
};


#endif // ISTORAGEVALUE_H_DEF
