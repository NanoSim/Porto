#ifndef QGENERIC_H_DEF
#define QGENERIC_H_DEF

#include <QObject>
#include <QString>
#include <QSharedPointer>
#include "qentity.h"
#include "qstorage.h"

class QGenericEntity : public QEntity

{
  Q_OBJECT
public:
  explicit QGenericEntity(QObject *parent = nullptr);
  explicit QGenericEntity(QGenericEntity const &other);
  QGenericEntity(QString const &id, QObject *parent = nullptr);  
  virtual ~QGenericEntity();
 
public slots:
  QString id() const;  
  QString metaName() const;
  QString metaNamespace() const;
  QString metaVersion() const;

  void setSchema(QString const&);
  void debug() const;
  QVariant property(QString const&) const;
  void setProperty(QString const&, QVariant const&);
};

#endif // QGENERIC_H_DEF
