#include "qh5datatype.h"

QH5Datatype :: QH5Datatype (QObject *parent)
  : QObject (parent)
  , datatypeId (H5T_NATIVE_HERR)
{}

QH5Datatype :: QH5Datatype (hid_t i, QObject *parent)
  : QObject (parent)
  , datatypeId (i)
{}

QH5Datatype :: QH5Datatype(QH5Datatype &&other)
  : QObject (std::move(other.parent()))
  , datatypeId (std::move(other.datatypeId))
{
}

QH5Datatype :: ~QH5Datatype()
{}

hid_t QH5Datatype :: id() const
{
  return datatypeId;
}

void QH5Datatype :: setId(hid_t i)
{
  datatypeId = i;
}

hid_t QH5Datatype :: toH5FileType(QH5Datatype::Type type)
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

hid_t QH5Datatype :: toH5MemoryType(QH5Datatype::Type type)
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
