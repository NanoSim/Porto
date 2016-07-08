#ifndef QSTORAGE_H_DEF
#define QSTORAGE_H_DEF

#include <QObject>
#include <QString>
#include <Soft>
#include "qentity.h"

class QStorage : public QObject
{
  Q_OBJECT
public:
  QStorage(QString const &driver,
	   QString const &uri,
	   QString const &options = QString());
  virtual ~QStorage();

public slots:
  void save (QEntity *entity);
  void load (QEntity *entity);

private:
  QScopedPointer<soft::Storage> storage;
};


#endif // QSTORAGE_H_DEF
