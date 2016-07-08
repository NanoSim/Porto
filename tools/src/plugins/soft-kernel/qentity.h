#ifndef QENTITY_H_DEF
#define QENTITY_H_DEF

#include <QObject>
#include <QString>
#include <QSharedPointer>
#include "genericentity.h"

namespace soft { class IEntity; }

class QEntity : public QObject
{
  Q_OBJECT
public:
  explicit QEntity(QObject *parent = nullptr);
  QEntity(soft::IEntity*, QObject *parent = nullptr);
  virtual ~QEntity() = 0; 
  
  virtual QString id() const;  
  virtual QString metaName() const;
  virtual QString metaNamespace() const;
  virtual QString metaVersion() const;
  soft::IEntity *ref() const;
protected:
  friend class QStorage;
  QSharedPointer<soft::IEntity> entity;  
};

#endif // QENTITY_H_DEF
