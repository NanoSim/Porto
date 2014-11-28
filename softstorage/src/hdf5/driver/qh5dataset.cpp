#include <QtDebug>
#include "qh5file.h"
#include "qh5dataspace.h"
#include "qh5dataset.h"

SOFT_BEGIN_NAMESPACE
H5_BEGIN_NAMESPACE

QH5Dataset :: QH5Dataset (QObject *parent)
  : QObject (parent)
  , datasetId (H5T_NATIVE_HERR)
  , datatype(QH5Datatype::Undefined)
{}

QH5Dataset :: ~QH5Dataset()
{
   if (datasetId != H5T_NATIVE_HERR) {
      H5Dclose (datasetId);
   }
}

QH5Dataset :: QH5Dataset (QH5Dataset &&other)
   : datasetId(std::move(other.datasetId))
   , datatype(std::move(other.datatype))
{}

hid_t QH5Dataset :: id() const
{
  return this->datasetId;
}

static hid_t openDataset(QH5File *file, QH5Dataspace *dataspace, QString const &path, QH5Datatype::Type dtype)
{
  QTextStream(stderr) << "opening dataset: " << endl;
  if (file->intent() == QH5File::ReadWrite) {
    return H5Dcreate (file->id(),
		      qPrintable(path),
		      QH5Datatype::toH5FileType(dtype),
		      dataspace->id(),
		      H5P_DEFAULT,
		      H5P_DEFAULT,
		      H5P_DEFAULT);
  }
  else {
    return H5Dopen2 (file->id(),
		     qPrintable(path),
		     H5P_DEFAULT);
  }
}

QH5Dataset :: QH5Dataset (QH5File *file, QH5Dataspace *dataspace, QString const &path, QH5Datatype::Type dtype, QObject *parent)
  : QObject (parent)
  , datasetId (openDataset (file, dataspace, path, dtype))
  , datatype(dtype)
{}

QH5Dataset &QH5Dataset :: operator=(QH5Dataset const &other)
{
   datasetId = other.datasetId;
   return *this;
}

bool QH5Dataset :: write (void const *data)
{
   auto status = H5Dwrite (datasetId, 
			   QH5Datatype::toH5FileType(datatype),
			   H5S_ALL, 
			   H5S_ALL, 
			   H5P_DEFAULT, 
			   data);
   return (status >= 0);
}

bool QH5Dataset :: read (void *data)
{
   auto status = H5Dread (datasetId, 
			  QH5Datatype::toH5MemoryType(datatype),
			  H5S_ALL, 
			  H5S_ALL, 
			  H5P_DEFAULT, 
			  data);
   return (status >= 0);
}

H5_END_NAMESPACE
SOFT_END_NAMESPACE
