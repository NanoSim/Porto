#ifndef QH5DATASPACE_H_DEF
#define QH5DATASPACE_H_DEF

#include <QObject>
#include <QVector>
#include <hdf5.h>
#include "h5ns.h"

SOFT_BEGIN_NAMESPACE
H5_BEGIN_NAMESPACE

class QH5Dataspace : public QObject
{ 
  Q_OBJECT
public:
  explicit QH5Dataspace (QObject *parent = nullptr);
  QH5Dataspace (QVector<int> const &dims, QObject * parent = nullptr);
  QH5Dataspace (QH5Dataspace &&other);
  virtual ~QH5Dataspace();

protected:
  friend class QH5;
  friend class QH5Dataset;
  friend class QH5Attribute;
public:
  hid_t id() const;
  
private:
  hid_t dataspaceId;
};

H5_END_NAMESPACE
SOFT_END_NAMESPACE

#endif // QH5DATASPACE_H_DEF
