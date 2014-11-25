#ifndef QH5PROPERTY_H_DEF
#define QH5PROPERTY_H_DEF

#include <QObject>
#include <hdf5.h>

class QH5Property : public QObject
{
  Q_OBJECT
  
public:
  QH5Property (hid_t propertyClass, QObject *parent = nullptr);
  virtual ~QH5Property();

protected:
  friend class QH5;
  hid_t id() const;

private:
  explicit QH5Property (QObject *parent = nullptr);
  hid_t propertyId;  
};

#endif // QH5PROPERTY_H_DEF
