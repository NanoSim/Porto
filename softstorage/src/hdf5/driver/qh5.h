#ifndef QH5_H_DEF
#define QH5_H_DEF

#include <QObject>
#include "qh5dataspace.h"
#include "qh5dataset.h"
#include "qh5file.h"
#include "qh5datatype.h"
#include "qh5group.h"

class QH5 : public QObject
{
  Q_OBJECT
public:
  QH5 (QObject *parent = nullptr);
  virtual ~QH5();
  
		
public slots:
  bool create(QString const &name);
  QH5Dataspace *createSimpleDataspace(QVector<int> const &dims);
  QH5Dataset *createDataset(QString const &path, QH5Dataspace *dataspace, QH5Datatype dtype);
  QH5Dataset *createDataset(QString const &path, QH5Group *group, QH5Dataspace *dataspace, QH5Datatype dtype);
  QH5Dataset *createDataset(QString const &path, QString const &groupName, QH5Dataspace *dataspace, QH5Datatype dtype);
  QH5Group *createGroup(QString const &groupName);
  bool write (QString const &path, QVariant const &v);
private:
  class Private;
  Private *d;
};

#endif // QH5_H_DEF
