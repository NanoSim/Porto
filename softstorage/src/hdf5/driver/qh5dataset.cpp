#include <QtDebug>
#include "qh5file.h"
#include "qh5dataspace.h"
#include "qh5dataset.h"

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

static hid_t datatypeToHDF5MemoryType (QH5Datatype type)
{
   switch (type) {
      case QH5Datatype::Int32:
	 return H5T_NATIVE_INT;
      case QH5Datatype::Int16:
	 return H5T_NATIVE_SHORT;
      case QH5Datatype::Int64:
	 return H5T_NATIVE_LLONG;
      case QH5Datatype::UInt32:
	 return H5T_NATIVE_UINT;
      case QH5Datatype::UInt16:
	 return H5T_NATIVE_USHORT;
      case QH5Datatype::UInt64:
	 return H5T_NATIVE_ULLONG;
      case QH5Datatype::Float:
	 return H5T_NATIVE_FLOAT;
      case QH5Datatype::Double:
	 return H5T_NATIVE_DOUBLE;
      case QH5Datatype::String:
	return H5T_NATIVE_CHAR;
      default:
	return H5T_NATIVE_HERR;
   }
}

static hid_t datatypeToHDF5FileType (QH5Datatype type)
{
  hid_t datatype = H5T_NATIVE_HERR;
   switch (type) {
      case QH5Datatype::Int32:
	 return H5T_STD_I32LE;
      case QH5Datatype::Int16:
	 return H5T_STD_I16LE;
      case QH5Datatype::Int64:
	 return H5T_STD_I64LE;
      case QH5Datatype::UInt32:
	 return H5T_STD_U32LE;
      case QH5Datatype::UInt16:
	 return H5T_STD_U16LE;
      case QH5Datatype::UInt64:
	 return H5T_STD_U64LE;
      case QH5Datatype::Float:
	 return H5T_IEEE_F32LE;
      case QH5Datatype::Double:
	 return H5T_IEEE_F64LE;
      case QH5Datatype::String:
	datatype = H5Tcopy(H5T_C_S1);
	H5Tset_size (datatype, H5T_VARIABLE);
	return datatype;
      default:
	return datatype;
   }
}

QH5Dataset :: QH5Dataset (QH5File *file, QH5Dataspace *dataspace, QString const &path, QH5Datatype dtype, QObject *parent)
  : QObject (parent)
  , datasetId (H5Dcreate (file->id(),
			  qPrintable(path),
			  datatypeToHDF5FileType(dtype),
			  dataspace->id(),
			  H5P_DEFAULT,
			  H5P_DEFAULT,
			  H5P_DEFAULT))
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
			   datatypeToHDF5FileType(datatype),
			   H5S_ALL, 
			   H5S_ALL, 
			   H5P_DEFAULT, 
			   data);
   return (status >= 0);
}

bool QH5Dataset :: read (void *data)
{
   auto status = H5Dread (datasetId, 
			  datatypeToHDF5MemoryType(datatype),
			  H5S_ALL, 
			  H5S_ALL, 
			  H5P_DEFAULT, 
			  data);
   return (status >= 0);
}
