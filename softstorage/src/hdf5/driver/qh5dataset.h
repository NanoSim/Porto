#ifndef QH5DATASET_H_DEF
#define QH5DATASET_H_DEF

#include <QObject>
#include <hdf5.h>
#include "qh5datatype.h"

class QH5Dataspace;
class QH5File;
class QString;

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
   hid_t id() const;
  
private:
   friend class QH5;
   friend class QH5Attribute;
   hid_t datasetId;
   QH5Datatype::Type datatype;
};

#endif // QH5DATASET_H_DEF
