#include <collection.h> // SOFT5 collection
#include "qcollection.h"

QCollection :: QCollection(QObject *parent)
  : QEntity(new soft::Collection(), parent)
{}

QCollection :: QCollection(QString const &id, QObject *parent)
  : QEntity(new soft::Collection(qPrintable(id)), parent)
{}

QCollection :: ~QCollection()
{}

QString QCollection :: id() const
{
  return QEntity::id();
}

QString QCollection :: metaName() const
{
  return QEntity::metaName();
}

QString QCollection :: metaNamespace() const
{
  return QEntity::metaNamespace();
}

QString QCollection :: metaVersion() const
{
  return QEntity::metaVersion();
}

QString QCollection :: name() const
{
  soft::Collection *ref = dynamic_cast<soft::Collection*>(entity.data());
  return QString::fromStdString(ref->name());
}

QString QCollection :: version() const
{
  soft::Collection *ref = dynamic_cast<soft::Collection*>(entity.data());
  return QString::fromStdString(ref->version());
}

int QCollection :: numEntities() const
{
  soft::Collection *ref = dynamic_cast<soft::Collection*>(entity.data());
  return ref->numEntities();
}

void QCollection :: setName(QString const &name)
{
  soft::Collection *ref = dynamic_cast<soft::Collection*>(entity.data());
  ref->setName(name.toStdString());
}

void QCollection :: setVersion(QString const &version)
{
  soft::Collection *ref = dynamic_cast<soft::Collection*>(entity.data());
  ref->setVersion(version.toStdString());
}

void QCollection :: registerEntity(QString const &label, QEntity *ent)
{
  soft::Collection *ref = dynamic_cast<soft::Collection*>(entity.data());
  ref->registerEntity(label.toStdString(), ent->ref());
}

void QCollection :: addEntity(QString const &label,
                 QString const &name,
                 QString const &version,
                 QString const &ns,
                 QString const &uuid)
{
  soft::Collection *ref = dynamic_cast<soft::Collection*>(entity.data());
  ref->addEntity(label.toStdString(), name.toStdString(), version.toStdString(), ns.toStdString(), uuid.toStdString());
}


QString QCollection :: entityId(QString const &label) const
{
  std::string n,v,s,u;
  soft::Collection *ref = dynamic_cast<soft::Collection*>(entity.data());
  ref->findEntity(label.toStdString(), n, v, s, u);
  return QString::fromStdString(u);
}

QString QCollection :: entityName(QString const &label) const
{
  std::string n,v,s,u;
  soft::Collection *ref = dynamic_cast<soft::Collection*>(entity.data());
  ref->findEntity(label.toStdString(), n, v, s, u);
  return QString::fromStdString(n);
}

QString QCollection :: entityVersion(QString const &label) const
{
  std::string n,v,s,u;
  soft::Collection *ref = dynamic_cast<soft::Collection*>(entity.data());
  ref->findEntity(label.toStdString(), n, v, s, u);
  return QString::fromStdString(v);
}

QString QCollection :: entityNamespace(QString const &label) const
{
  std::string n,v,s,u;
  soft::Collection *ref = dynamic_cast<soft::Collection*>(entity.data());
  ref->findEntity(label.toStdString(), n, v, s, u);
  return QString::fromStdString(s);
}

void QCollection :: findEntity(QString const &label,
                               QString &name,
                               QString &version,
                               QString &ns,
                               QString &uuid) const
{
  std::string n,v,s,u;
  soft::Collection *ref = dynamic_cast<soft::Collection*>(entity.data());
  ref->findEntity(label.toStdString(), n, v, s, u);
  name = QString::fromStdString(n);
  version = QString::fromStdString(v);
  ns = QString::fromStdString(s);
  uuid = QString::fromStdString(u);
}

void QCollection :: attachEntity(QString const &label, QEntity *ent)
{
  soft::Collection *ref = dynamic_cast<soft::Collection*>(entity.data());
  ref->attachEntity(label.toStdString(), ent->ref());
}



void QCollection :: addDim(QString const &label,
                           QString const &description)
{
  soft::Collection *ref = dynamic_cast<soft::Collection*>(entity.data());
  ref->addDim(label.toStdString(), description.toStdString());
}

void QCollection :: connect(QString const &subject,
             QString const &predicate,
             QString const &object)
{
  soft::Collection *ref = dynamic_cast<soft::Collection*>(entity.data());
  ref->connect(subject.toStdString(), predicate.toStdString(), object.toStdString());
}


void QCollection :: addRelation(QString const &subject,
                   QString const &predicate,
                   QString const &object)
{
  soft::Collection *ref = dynamic_cast<soft::Collection*>(entity.data());
  ref->addRelation(subject.toStdString(), predicate.toStdString(), object.toStdString());
}

void QCollection :: addDimMap(QString const &label,
                 QString const &entityDim,
                 QString const &collectionDim)
{
  soft::Collection *ref = dynamic_cast<soft::Collection*>(entity.data());
  ref->addDimMap(label.toStdString(), entityDim.toStdString(), collectionDim.toStdString());
}


int QCollection :: numRelations() const
{
  soft::Collection *ref = dynamic_cast<soft::Collection*>(entity.data());
  return ref->numRelations();
}

QStringList QCollection :: findRelations(QString const &subject, QString const &object)
{
  soft::Collection *ref = dynamic_cast<soft::Collection*>(entity.data());
  auto relations = ref->findRelations(subject.toStdString(),
                                    object.toStdString());
  QStringList ret;
  for (auto relation: relations) {
    ret << QString::fromStdString(relation);
  }
  return ret;
}
