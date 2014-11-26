#ifndef QH5ATTRIBUTE_H_DEF
#define QH5ATTRIBUTE_H_DEF

#include <QObject>
#include <hdf5.h>

class QH5Dataset;
class QH5Dataspace;
class QH5Datatype;
class QH5Attribute : public QObject
{
  Q_OBJECT
public:
  explicit QH5Attribute (QObject *parent = nullptr);
  QH5Attribute (QH5Dataset *dataset,
		QString const &attributeName,
		QH5Datatype *datatype,
		QH5Dataspace *dataspace,
		QObject *parent = nullptr);
  QH5Attribute (QH5Attribute &&other);
  virtual ~QH5Attribute();

protected:
  friend class QH5;
  hid_t id() const;

private:
  hid_t attributeId;
};

#endif // QH5ATTRIBUTE_H_DEF
