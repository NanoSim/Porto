#ifndef QH5DATASET_H_DEF
#define QH5DATASET_H_DEF

#include <QObject>
#include <hdf5.h>
#include "qh5datatype.h"
#include "h5ns.h"

class QString;

SOFT_BEGIN_NAMESPACE
H5_BEGIN_NAMESPACE

class QH5Dataspace;
class QH5File;

class QH5Dataset : public QObject
{
   Q_OBJECT
public:
   explicit QH5Dataset (QObject *parent = nullptr);
  QH5Dataset (QH5File *file, QH5Dataspace *dataspace, QString const &path, QH5Datatype::Type dtype, QObject *parent = nullptr);
   QH5Dataset (QH5Dataset &&other);
   QH5Dataset &operator=(QH5Dataset const &other);
   virtual ~QH5Dataset();

public slots:
   bool write (void const *data);
   bool read (void *data);

protected:

public:
   hid_t id() const;
  
private:
   friend class QH5;
   friend class QH5Attribute;
   hid_t datasetId;
   QH5Datatype::Type datatype;
};

H5_END_NAMESPACE
SOFT_END_NAMESPACE

#endif // QH5DATASET_H_DEF
