#include "qh5file.h"
#include "qh5group.h"

QH5Group :: QH5Group (QObject *parent)
  : QObject (parent)
  , groupId (H5T_NATIVE_HERR)
{}

QH5Group :: QH5Group (QH5File *file, QString const &groupName, QObject *parent)
  : QObject (parent)
  , groupId (H5Gcreate(file->id(),
		       qPrintable(groupName),
		       H5P_DEFAULT,
		       H5P_DEFAULT,
		       H5P_DEFAULT))
  , groupName (groupName)
{}

QString QH5Group :: name() const
{
  return groupName;
}

hid_t QH5Group :: id() const
{
   return groupId;
}

QH5Group :: ~QH5Group()
{
  if (groupId >= 0) {
    auto status = H5Gclose (groupId);
    Q_UNUSED(status);
  }
}
