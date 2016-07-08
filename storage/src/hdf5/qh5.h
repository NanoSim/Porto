#ifndef QH5_H_DEF
#define QH5_H_DEF

#include <QObject>
#include <QVariantMap>
#include "qh5dataspace.h"
#include "qh5dataset.h"
#include "qh5file.h"
#include "qh5datatype.h"
#include "qh5group.h"
#include "h5ns.h"

SOFT_BEGIN_NAMESPACE
H5_BEGIN_NAMESPACE

class QH5 : public QObject
{
  Q_OBJECT
public:
  QH5 (QObject *parent = nullptr);
  virtual ~QH5();
  
		
public slots:
  bool create(QString const &name, QVariantMap const &map = QVariantMap());
  bool open(QString const &name, bool readonly=true);
  QH5Dataspace *createSimpleDataspace(QVector<int> const &dims);
  QH5Dataset *createDataset(QString const &path, QH5Dataspace *dataspace, QH5Datatype::Type dtype);
  QH5Dataset *createDataset(QString const &path, QH5Group *group, QH5Dataspace *dataspace, QH5Datatype::Type dtype);
  QH5Dataset *createDataset(QString const &path, QString const &groupName, QH5Dataspace *dataspace, QH5Datatype::Type dtype);
  QH5Group *createGroup(QString const &groupName);
  bool addGroup (QString const &key, QVariantMap const &map = QVariantMap());
  bool close();
  bool info();
  QStringList datasets();
  static bool describeType (QVariant const &v);

public:
  QVariant read (QString const &key); 
  bool write (QString const &path, QVariant const &v, QVariantMap const &attributes = QVariantMap());

private:
  class Private;
  Private *d;
};

H5_END_NAMESPACE
SOFT_END_NAMESPACE

#endif // QH5_H_DEF
