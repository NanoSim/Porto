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

QH5File::HandleFlag QH5File :: intent() const
{ 
  unsigned intent;
  auto err = H5Fget_intent(fileId, &intent);
  if (err < 0) return QH5File::Undefined;
  if (intent == H5F_ACC_RDWR) return QH5File::ReadWrite;
  return QH5File::ReadOnly;
}

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

bool QH5File :: open (QString const &filename, bool readonly)
{
  fileId = H5Fopen(qPrintable(filename),
		   (readonly ? H5F_ACC_RDONLY : H5F_ACC_RDWR),
		   H5P_DEFAULT);
  return (fileId >= 0);
}

int QH5File :: objectCount() const
{
  auto size = H5Fget_obj_count (fileId, H5F_OBJ_ALL);
  return size;
}

bool QH5File :: flush()
{
  auto status = H5Fflush (fileId, H5F_SCOPE_GLOBAL);
  return (status >= 0);
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

bool QH5File :: close()
{
  auto status = H5Fclose(fileId);
  fileId = H5T_NATIVE_HERR;

  return (status >= 0);
}

H5_END_NAMESPACE
SOFT_END_NAMESPACE
