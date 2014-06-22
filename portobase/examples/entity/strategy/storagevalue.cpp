#include "storagevalue.h"
#include <QtCore/QJsonValue>
#include <QtCore/QString>
#include <string>

class StorageValue::Private
{
   friend class StorageValue;
   Private(QJsonValue const &v)
      : value(v)
   {}
   
   QJsonValue const value;
};

StorageValue :: StorageValue()
   : d(new StorageValue::Private(QJsonValue()))
{}

StorageValue :: StorageValue (StorageValue const& other)
   : d(new StorageValue::Private(other.d->value))
{}

StorageValue :: StorageValue (std::string const & v)
   : d(new StorageValue::Private(QString::fromStdString(v)))
{}

StorageValue :: StorageValue (int v) 
   : d(new StorageValue::Private(v))
{}

StorageValue :: StorageValue (double const &v) 
   : d(new StorageValue::Private(v))
{}

StorageValue :: ~StorageValue()
{}

bool StorageValue :: isInt() const
{
   return d->value.isDouble();
}

bool StorageValue :: isDouble() const
{
   return d->value.isDouble();
}

bool StorageValue :: isString() const
{
   return d->value.isString();
}

int StorageValue :: toInt() const
{
   return d->value.toInt();
}

double StorageValue :: toDouble() const
{
   return d->value.toDouble();
}

std::string StorageValue :: toString() const
{
   return d->value.toString().toStdString();
}

void *StorageValue :: ref() const
{
   return (void*)(&d->value);
}
