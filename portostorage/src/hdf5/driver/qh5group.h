#ifndef QH5GROUP_H_DEF
#define QH5GROUP_H_DEF

#include <QObject>
#include <hdf5.h>
#include "h5ns.h"

PORTO_BEGIN_NAMESPACE
H5_BEGIN_NAMESPACE

class QH5File;
class QH5Group : public QObject
{
public:
  explicit QH5Group (QObject *parent = nullptr);
  QH5Group (QH5File *file, QString const &groupName, QObject *parent = nullptr);
  virtual ~QH5Group();
		     
public slots:
  QString name() const;

protected:
  hid_t id() const;

private:
  friend class QH5;
  hid_t groupId;
  QString const groupName;
};

H5_END_NAMESPACE
PORTO_END_NAMESPACE

#endif // QH5GROUP_H_DEF
