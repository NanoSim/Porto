#include <QtCore/QJsonObject>
#include "storagevalue.h"
#include "storageobject.h"
#include "storagevalue.h"

class StorageObject::Private
{
   friend class StorageObject;
   Private() {}

   QJsonObject object;
};

StorageObject :: StorageObject()
   : d (new StorageObject::Private())
{}

void StorageObject :: insert(const char *key, const StorageValue &value)
{
   auto jsonValue = (QJsonValue*)value.ref();
   d->object.insert (QString::fromLocal8Bit(key), *jsonValue);
}

StorageValue StorageObject :: find(const char *key) const
{   
   auto it = d->object.find(QString::fromLocal8Bit(key));
   if (it != d->object.end()) {
      switch (it.value().type()) {
	 case QJsonValue::Double:
	    return StorageValue(it.value().toDouble());

	 case QJsonValue::String:
	 default:
	    return StorageValue(it.value().toString().toStdString());
      }
   }
   return StorageValue();
}

