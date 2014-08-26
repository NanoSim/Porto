#ifndef STORAGEVALUE_H_DEF
#define STORAGEVALUE_H_DEF

#include "storageobject.h"
#include "istoragevalue.h"
class StorageValue : public IStorageValue
{   
public:      
   StorageValue();
   StorageValue (StorageValue const & other);
   explicit StorageValue (const std::string &v);
   explicit StorageValue (int v);
   explicit StorageValue (double const &v);
   virtual ~StorageValue();

   virtual bool isInt() const;
   virtual bool isDouble() const;
   virtual bool isString() const;
   
   virtual int toInt() const;
   virtual double toDouble() const;
   virtual std::string toString() const;

protected:
   friend class StorageObject;
   void *ref() const;
private:
   class Private;
   Private *d;  
};

#endif // STORAGEVALUE_H_DEF
