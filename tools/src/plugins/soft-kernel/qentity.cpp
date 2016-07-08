#include "qentity.h"

QEntity :: QEntity(QObject *parent)
  : QObject(parent)
{}

QEntity :: QEntity(soft::IEntity *entity, QObject *parent)
  : QObject(parent)
  , entity(entity)
{}

QEntity :: ~QEntity()
{}

QString QEntity :: id() const
{
  return QString::fromStdString(entity->id());
}

QString QEntity :: metaName() const
{
  return QString::fromStdString(entity->metaName());
}

QString QEntity :: metaNamespace() const
{
  return QString::fromStdString(entity->metaNamespace());
}

QString QEntity :: metaVersion() const
{
  return QString::fromStdString(entity->metaVersion());
}

soft::IEntity *QEntity :: ref() const
{
  return entity.data();
}

