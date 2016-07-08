#include "qgenericentity.h"

QGenericEntity :: QGenericEntity(QObject *parent)
  : QEntity(new soft::GenericEntity())
{}
  
QGenericEntity :: QGenericEntity(QString const &id, QObject *parent)
  : QEntity(new soft::GenericEntity(qPrintable(id)), parent)
{}

QGenericEntity :: ~QGenericEntity()
{}

void QGenericEntity :: debug() const
{
  soft::GenericEntity *ref = dynamic_cast<soft::GenericEntity*>(entity.data());
  ref->debug();
}

void QGenericEntity :: setSchema(QString const& schema)
{
  soft::GenericEntity *ref = dynamic_cast<soft::GenericEntity*>(entity.data());
  ref->setSchema(schema.toStdString());
}

QVariant QGenericEntity :: property(QString const &key) const
{
  soft::GenericEntity *ref = dynamic_cast<soft::GenericEntity*>(entity.data());
  return ref->property(key);
}

void QGenericEntity :: setProperty(QString const& key, QVariant const &value)
{
  soft::GenericEntity *ref = dynamic_cast<soft::GenericEntity*>(entity.data());
  ref->setProperty(key,value);
}

QString QGenericEntity :: id() const
{
  return QEntity::id();
}

QString QGenericEntity :: metaName() const
{
  return QEntity::metaName();
}

QString QGenericEntity :: metaNamespace() const
{
  return QEntity::metaNamespace();
}

QString QGenericEntity :: metaVersion() const
{
  return QEntity::metaVersion();
}

