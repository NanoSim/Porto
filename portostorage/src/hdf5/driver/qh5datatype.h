#ifndef QH5DATATYPE_H_DEF
#define QH5DATATYPE_H_DEF

#include <QObject>
#include <hdf5.h>
#include "h5ns.h"

PORTO_BEGIN_NAMESPACE
H5_BEGIN_NAMESPACE

class QH5Datatype : public QObject
{
  Q_OBJECT
  Q_ENUMS (Type)
public:
  explicit QH5Datatype (QObject *parent = nullptr);
  QH5Datatype (hid_t i, QObject *parent = nullptr);
  QH5Datatype(QH5Datatype &&other);
  virtual ~QH5Datatype();
  
  enum Type {Undefined=-1, Int32, Int16, Int64, UInt32, UInt16, UInt64, Float, Double, String};

public slots:
  hid_t id() const;
  void setId(hid_t);
  static hid_t toH5FileType(Type t);
  static hid_t toH5MemoryType(Type t);
  
private:
  hid_t datatypeId;
};

H5_END_NAMESPACE
PORTO_END_NAMESPACE

#endif // QH5DATATYPE_H_DEF
