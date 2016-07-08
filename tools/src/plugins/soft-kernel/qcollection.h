#ifndef QCOLLECTION_H_DEF
#define QCOLLECTION_H_DEF

#include "qentity.h"

class QCollection : public QEntity
{
  Q_OBJECT
public:
  explicit QCollection(QObject *parent = nullptr);
  explicit QCollection(QCollection const &other);
  QCollection(QString const &id, QObject *parent = nullptr);  
  virtual ~QCollection();
  
public slots:
  virtual QString id() const;  
  virtual QString metaName() const;
  virtual QString metaNamespace() const;
  virtual QString metaVersion() const;

  void setName(QString const &);
  void setVersion(QString const &);
  QString name() const;
  QString version() const;
  void registerEntity(QString const &label, QEntity *entity);
  void addEntity(QString const &label,
                 QString const &name,
                 QString const &version,
                 QString const &ns,
                 QString const &uuid);

  QString entityId(QString const &label) const;
  QString entityName(QString const &label) const;
  QString entityVersion(QString const &label) const;
  QString entityNamespace(QString const &label) const;
  
  void findEntity(QString const &label,
		  QString &name,
                  QString &version,
                  QString &ns,
                  QString &uuid) const;
  void attachEntity(QString const &label, QEntity *entity);
  

  void addDim(QString const &label,
              QString const &description = QString());
  // TODO: Not fully implemented.
  void connect(QString const &subject,
               QString const &predicate,
               QString const &object);
  
  void addRelation(QString const &subject,
                   QString const &predicate,
                   QString const &object);
  void addDimMap(QString const &label,
                 QString const &entityDim,
                 QString const &collectionDim);

  QStringList findRelations(QString const &subject, QString const &object);
  int numEntities() const;
  int numRelations() const;
};

#endif
