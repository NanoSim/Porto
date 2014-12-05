#ifndef STORAGEOBJECT_H_DEF
#define STORAGEOBJECT_H_DEF

class StorageValue;
class StorageObject
{
public:
   StorageObject();
   void insert(const char *key, const StorageValue &value);
   StorageValue find(const char *key) const;

private:
   class Private;
   Private *d;
};

#endif // STORAGEOBJECT_H_DEF
