#include <QString>
#include <QtDebug>
#include "qh5file.h"

SOFT_BEGIN_NAMESPACE
H5_BEGIN_NAMESPACE

QH5File :: QH5File (QObject *parent)
  : QObject (parent)
  , fileId (H5T_NATIVE_HERR)
{}

QH5File :: QH5File (hid_t id, QObject *parent)
   : QObject (parent)
   , fileId (id)
{}

QH5File :: QH5File (QString const &filename, QObject *parent)
  : QObject (parent)
  , fileId (H5T_NATIVE_HERR)
  , name (filename)
{}

QH5File ::  ~QH5File()
{
  if (fileId != H5T_NATIVE_HERR) {
    auto status = H5Fclose(fileId);
    qDebug() << "closing" << status;
    Q_UNUSED (status);
  }
}

bool QH5File :: create(QString const &filename)
{
  fileId = H5Fcreate(qPrintable(filename),
		     H5F_ACC_TRUNC,
		     H5P_DEFAULT,
		     H5P_DEFAULT);
  return true;
}

bool QH5File :: create()
{
  return this->create(name);
}

hid_t QH5File :: id() const
{
  return this->fileId;
}

void QH5File :: setId(hid_t i)
{
   this->fileId = i;
}

H5_END_NAMESPACE
SOFT_END_NAMESPACE
