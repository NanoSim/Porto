#include <QtCore>
#include <vector>
#include <hdf5.h>
#include <memory>
#include "qh5.h"

SOFT_BEGIN_NAMESPACE
H5_BEGIN_NAMESPACE

static bool writeAttributeString (hid_t dataset, QString const &key, QString const &value)
{
   auto dataspace = H5Screate (H5S_SCALAR);
   auto datatype = H5Tcopy(H5T_C_S1);
   H5Tset_size(datatype, value.length());
   auto attr = H5Acreate(dataset, qPrintable(key), datatype, dataspace, H5P_DEFAULT,H5P_DEFAULT);
   auto status = H5Awrite(attr, datatype, value.toLocal8Bit().constData());
   H5Sclose (dataspace);
   H5Aclose (attr);
   return (status >= 0);
}

static bool writeAttributeInt (hid_t dataset, QString const &key, int value)
{
   auto datatype = H5T_STD_I32LE;
   auto dataspace = H5Screate (H5S_SCALAR);
   auto attr = H5Acreate(dataset, qPrintable(key), datatype, dataspace, H5P_DEFAULT, H5P_DEFAULT);
   auto status = H5Awrite(attr, datatype, &value);
   H5Aclose(attr);
   H5Sclose(dataspace);
   return (status >= 0);
}

static bool writeAttribute (hid_t dataset, QString const &key, QVariant const &value)
{
   switch (value.type()) {
      case QMetaType::QString:
	 return writeAttributeString (dataset, key, value.toString());
      case QMetaType::Int:
	 return writeAttributeInt (dataset, key, value.toInt());
      default:
	 return false;
   } 
}

static bool writeString(hid_t file_id, QString const &path, QString const &buff, QVariantMap const &attr)
{
   auto datatype = H5Tcopy(H5T_C_S1);
   H5Tset_size(datatype, buff.length());
   H5Tset_strpad(datatype, H5T_STR_NULLTERM);

   auto dataspace = H5Screate(H5S_SCALAR);
   auto dataset = H5Dcreate(file_id, qPrintable(path), datatype, dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
   auto status = H5Dwrite(dataset, datatype, H5S_ALL, H5S_ALL, H5P_DEFAULT, buff.toLocal8Bit().constData());

   for (auto a : attr.keys()) {
      writeAttribute(dataset, a, attr[a]);
   }

   H5Dclose(dataset);
   H5Sclose(dataspace);
  
   return (status >= 0);
}

static bool writeStringList(hid_t file_id, QString const &path, QStringList const &xs)
{
   auto datatype = H5Tcopy(H5T_C_S1);
   H5Tset_size (datatype, H5T_VARIABLE);

   std::vector<hsize_t> sizes(1);
   sizes[0] = xs.size();

   auto dataspace = H5Screate_simple(1, &sizes[0], nullptr);
   auto dataset = H5Dcreate(file_id, qPrintable(path), datatype, dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

   auto joined = xs.join("\0");
   auto output = joined.toLocal8Bit().constData();
   auto status = H5Dwrite(dataset, datatype, H5S_ALL, H5S_ALL, H5P_DEFAULT, output);

   H5Dclose(dataset);
   H5Sclose(dataspace);
  
   return (status >= 0);
}

static bool writeInt(hid_t file_id, QString const &path, int value)
{
   auto datatype  = H5T_STD_I32LE;
   auto dataspace = H5Screate(H5S_SCALAR);
   auto dataset   = H5Dcreate(file_id, qPrintable(path), datatype, dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
   auto status    = H5Dwrite(dataset, datatype, H5S_ALL, H5S_ALL, H5P_DEFAULT, &value);
   H5Dclose(dataset);
   H5Sclose(dataspace);
   return (status >= 0);
}

static bool writeDouble(hid_t file_id, QString const &path, double const & value)
{
   auto datatype  = H5T_IEEE_F64LE;
   auto dataspace = H5Screate(H5S_SCALAR);
   auto dataset   = H5Dcreate(file_id, qPrintable(path), datatype, dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
   auto status    = H5Dwrite(dataset, datatype, H5S_ALL, H5S_ALL, H5P_DEFAULT, &value);
   H5Dclose(dataset);
   H5Sclose(dataspace);
   return (status >= 0); 
}

static QVector<int> getListDims(QVariant const &v)
{
   if (v.type() == QMetaType::QVariantList) {
      QVector<int> dim = QVector<int>() << v.toList().count() << getListDims(v.toList().first());
      return dim;
   }
   return QVector<int>();
}

static QVariant::Type getListType(QVariant const &v)
{
   if (v.type() != QMetaType::QVariantList) {
      return v.type();
   }
   return getListType (v.toList().first());
}

template <class T>
QVector<T> stdFlat (QVariant const &value) 
{
   QVector<T> vector;
   if (value.type() !=QMetaType::QVariantList) {
      vector.append (value.value<T>());
   } else {
      for (auto v : value.toList()) {
	 vector << stdFlat<T> (v);
      }
   }
   return vector;
}

QStringList variantListToStringList(QVariant const &value)
{
   QStringList ret;
   if (value.type() != QMetaType::QVariantList) {
      ret.append(value.toString());
   } else {
      for (auto s : value.toList()) {
	 ret << variantListToStringList (s);
      }
   }
   return ret;
}

class StringListData
{
   char **data;

public:
   StringListData(QStringList const &input)
      : data (new char *[input.size() + 1])
   {
      int i  = 0;
      for (; i < input.size(); ++i) {
	 auto len = input.at(i).length() + 1;
	 data[i] = new char [len];
	 memcpy(data[i], input.at(i).toLocal8Bit().constData(), len);
      }
      data[input.size()] = nullptr;
   }

   const char **constData() const {
      return const_cast<const char **>(data);
   }
  
   ~StringListData()
   {
      int i = 0;
      while (data [i])
	 delete data[i++];
      delete data;
   }
}; // class StringListData

static bool writeVariantStringList(QH5File *file, QString const &path, QVariantList const &lst)
{
   auto datatype  = QH5Datatype::String;
   auto dims      = getListDims(lst);
   auto dataspace = new QH5Dataspace(dims, file);
   auto dataset   = new QH5Dataset(file, dataspace, path, datatype, dataspace); 
   QStringList input = variantListToStringList(lst);
   StringListData sd(input);
   auto result = dataset->write(sd.constData());  
   return result;
}

static bool writeVariantIntList(QH5File *file, QString const &path, QVariantList const &list)
{
   auto datatype  = QH5Datatype::Int32;
   auto dims      = getListDims(list);
   auto dataspace = new QH5Dataspace(dims);
   auto dataset   = new QH5Dataset(file, dataspace, path, datatype, dataspace); 
   auto qvec      = stdFlat<int>(list);
   auto ptr       = qvec.data();
   auto status    = dataset->write(ptr);
  
   delete dataspace;
   return (status >= 0);
}

static bool writeVariantDoubleList(QH5File *file, QString const &path, QVariantList const &list)
{
   auto datatype  = QH5Datatype::Double;
   auto dims      = getListDims(list);
   auto dataspace = new QH5Dataspace(dims);
   auto dataset   = new QH5Dataset(file, dataspace, path, datatype, dataspace); 
   auto qvec      = stdFlat<double>(list);
   auto ptr       = qvec.data();
   auto status    = dataset->write(ptr);
  
   delete dataspace;
   return (status >= 0);
}
static bool writeVariantList(QH5File *file, QString const &path, QVariantList const &list)
{

   auto type = getListType(list);
   switch (type) {
      case QMetaType::QString:
	 return writeVariantStringList(file, path, list);
	 break;
      case QMetaType::Int:
	 return writeVariantIntList(file, path, list);
	 break;
      case QMetaType::Double:
	 return writeVariantDoubleList(file, path, list);
	 break;
      default:
	 return false;
   }
}

class QH5::Private {
   friend class QH5;

   Private ()
      : file (nullptr)
   {}

   QH5File *file;
   QStringList ret;
};

QH5 :: QH5 (QObject *parent)
   : QObject (parent)
   , d (new QH5::Private ())
{
   Q_CHECK_PTR (d);
}

QH5 :: ~QH5()
{
   delete d->file;
   delete d;
}

bool QH5 :: create(QString const &filename, QVariantMap const &map)
{
   d->file = new QH5File(filename, this);
   d->file->create();

   for (auto key : map.keys()) {
      writeAttribute(d->file->id(), key, map.value(key));
   }

   return true;
}

bool QH5 :: open (QString const &filename)
{
   d->file = new QH5File(this);
   d->file->open(filename);
   return true;
}

QH5Dataspace *QH5 :: createSimpleDataspace(QVector<int> const &dims)
{
   auto dataspace = new QH5Dataspace (dims, this);
   return dataspace;
}

QH5Dataset *QH5 :: createDataset(QString const &path, QH5Dataspace *dataspace, QH5Datatype::Type dtype)
{
   auto dataset = new QH5Dataset(d->file, dataspace, path, dtype, this);
   return dataset;
}

QH5Dataset *QH5 :: createDataset(QString const &path, QH5Group *group, QH5Dataspace *dataspace, QH5Datatype::Type dtype)
{
   auto dataset = createDataset(group->name() + "/" + path, dataspace, dtype);
   return dataset;
}

QH5Dataset *QH5 :: createDataset(QString const &path, QString const &groupName, QH5Dataspace *dataspace, QH5Datatype::Type dtype)
{
   auto group = new QH5Group (d->file, groupName, this);
   auto dataset = createDataset(path, group, dataspace, dtype);
   return dataset;
}


QH5Group *QH5 :: createGroup (QString const &groupName)
{
   auto group = new QH5Group (d->file, groupName, this);
   return group;
}

bool QH5 :: addGroup (QString const &groupName, QVariantMap const &map)
{
   auto group = new QH5Group (d->file, groupName, this);

   for (auto key : map.keys()) {
      writeAttribute(group->id(), key, map.value(key));
   }

   return true;
}

bool QH5 :: write (QString const &path, QVariant const &value, QVariantMap const &attributes)
{
   switch (value.type()) {
      case QMetaType::QString:
	 return writeString(d->file->id(), path, value.toString(), attributes);
      case QMetaType::Int:
	 return writeInt(d->file->id(), path, value.toInt());
      case QMetaType::Double:
	 return writeDouble(d->file->id(), path, value.toDouble());
      case QMetaType::QStringList:
	 return writeStringList(d->file->id(), path, value.toStringList());  
      case QMetaType::QVariantList:
	 return writeVariantList(d->file, path, value.toList());
      default:
	 QTextStream(stderr) << "Undefined type";
	 return false;
   }
}

bool QH5 :: describeType (QVariant const &v)
{
   QTextStream(stdout) << v.typeName();
   if (v.type() == QMetaType::QVariantList) {
      QTextStream(stdout) << "[" << v.toList().count() << "] ";
      return describeType (v.toList().first());
   }
   QTextStream(stdout) << "\n";
   return true;
}



static herr_t op (hid_t loc_id, const char *name, const H5O_info_t *info,
		  void *op_data)
{
//   OpData *od = (OpData*)op_data;
   if (name[0] == '.') {
      QTextStream (stdout) << "Group" << endl;
   } else {
      switch (info->type) {
	 case H5O_TYPE_GROUP:
	    QTextStream (stdout) << "Group: " << name << endl;
	    break;
	 case H5O_TYPE_DATASET:
	    QTextStream (stdout) << "Dataset: " << name << endl;
	    break;
	 case H5O_TYPE_NAMED_DATATYPE:
	    QTextStream (stdout) << "Datatype: " << name << endl;
	    break;
	 default:
	    QTextStream (stdout) << "Unknown: " << name << endl;
      }
      return 0;
   }
}

static herr_t op_L (hid_t loc_id, const char *name, const H5L_info_t *info,
		    void *op_data)
{
   H5O_info_t infobuf;
   auto status = H5Oget_info_by_name (loc_id, name, &infobuf, H5P_DEFAULT);
   return op (loc_id, name, &infobuf, nullptr);
}

bool QH5 :: info()
{
   auto status = H5Ovisit(d->file->id(), H5_INDEX_NAME, H5_ITER_NATIVE,
			  op, nullptr);

   status = H5Lvisit (d->file->id(), H5_INDEX_NAME, H5_ITER_NATIVE, op_L, nullptr);
   return (status <= 0);
}

bool QH5 :: infoDataset2 (QString const &datasetName)
{
   auto dataset      = H5Dopen2(d->file->id(), qPrintable (datasetName), H5P_DEFAULT);
   auto datatype     = H5Dget_type (dataset);
   auto dclass       = H5Tget_class(datatype);
   auto filespace    = H5Dget_space(dataset);
   auto rank         = H5Sget_simple_extent_ndims(filespace);
   std::unique_ptr<hsize_t[]> dims(new hsize_t[rank]);
   std::unique_ptr<hvl_t[]> rdata(new hvl_t[rank]);
   auto status_n     = H5Sget_simple_extent_dims(filespace, dims.get(), nullptr);


   hsize_t stot = 1;
   QTextStream(stdout) << datasetName << " - rank: " << rank << ", dims:";
   for (int i = 0; i < rank; ++i) {
      stot *= dims[i];
      QTextStream(stdout) << dims[i] << " ";
   }

   QVector<double> data(stot);
   char *ptr = nullptr;
   auto memspace     = H5Screate_simple(rank, dims.get(), nullptr);

   hid_t status;
//   switch (dclass) {    
//      case H5T_FLOAT:
	 status = H5Dread(dataset, H5T_NATIVE_DOUBLE, memspace, filespace, H5P_DEFAULT, data.data());
	 QTextStream(stdout) << ", value; [";
	 for (auto v : data) {
	    QTextStream(stdout) << v << " ";
	 }
	 QTextStream(stdout) << "]";    
//	 break;
//      default:
//	 break;
//   }


   QTextStream(stdout) << "\n";
   return true;
}

bool QH5 :: infoDataset (QString const &datasetName)
{
   auto dataset = H5Dopen2(d->file->id(), qPrintable (datasetName), H5P_DEFAULT);
   auto datatype = H5Dget_type (dataset);
   auto dclass   = H5Tget_class(datatype);
   QTextStream(stdout) << datasetName;
   switch (dclass) {
      case H5T_INTEGER:
	 QTextStream(stdout) << " (int)";
	 break;
      case H5T_FLOAT:
	 QTextStream(stdout) << " (float)";
	 break;
      case H5T_STRING:
	 QTextStream(stdout) << " (string)";
	 break;
      case H5T_BITFIELD:
	 QTextStream(stdout) << " (bitfield)";
	 break;
      case H5T_OPAQUE:
	 QTextStream(stdout) << " (opaque)";
	 break;
      case H5T_COMPOUND:
	 QTextStream(stdout) << " (compound)";
	 break;
      case H5T_REFERENCE:
	 QTextStream(stdout) << " (reference)";
	 break;
      case H5T_ENUM:
	 QTextStream(stdout) << " (enum)";
	 break;
      case H5T_VLEN:
	 QTextStream(stdout) << " (vlen)";
	 break;
      case H5T_ARRAY:
      default:
	 QTextStream(stdout) << " (array)";
	 break;

   }

   auto size = H5Tget_size(datatype);
   QTextStream(stdout) << " (size: " <<  size << ")";
   QTextStream(stdout) << "\n";
   return true;
}

struct OpData {
   QStringList datasets;
};

herr_t myop(hid_t loc_id, const char *name, const H5L_info_t *info,void *op_data)
{  
   OpData *od = (OpData *)op_data;
   H5O_info_t  infobuf;
   auto status = H5Oget_info_by_name (loc_id, name, &infobuf, H5P_DEFAULT);
   switch (infobuf.type) {
      case H5O_TYPE_DATASET:
	 od->datasets << name;
	 break;    
      default:
	 break;
   }
   return status;
}

QVariant QH5 :: readArray(QString const &key)
{
   auto dataset   = H5Dopen2 (d->file->id(), qPrintable(key), H5P_DEFAULT);
   auto datatype  = H5Dget_type (dataset);
   auto dclass    = H5Tget_class (datatype);
   auto filespace = H5Dget_space (dataset);
   auto rank      = H5Sget_simple_extent_ndims(filespace);
   QVector <hsize_t> dims(rank);

   auto status    = H5Sget_simple_extent_dims(filespace, dims.data(), nullptr);
   auto memspace  = H5Screate_simple(rank, dims.data(), nullptr);
   QVector<double> vec(dims[0]);
   status         = H5Dread (dataset, H5T_NATIVE_DOUBLE, memspace, filespace, H5P_DEFAULT, vec.data());
   QVariantList list;
   for (auto v : vec) list << QVariant(v);

   return QVariant(list);
}

QVariant QH5 :: readStringArray(QString const &key) const
{
   auto dataset   = H5Dopen2 (d->file->id(), qPrintable(key), H5P_DEFAULT);
   auto datatype  = H5Dget_type (dataset);
   auto dclass    = H5Tget_class (datatype);
   auto filespace = H5Dget_space (dataset);
   auto rank      = H5Sget_simple_extent_ndims(filespace);
   if (rank > 1) return QStringList();

   QVector<hsize_t> dims(rank);
   auto ndims     = H5Sget_simple_extent_dims(filespace, dims.data(), nullptr);
   auto memspace  = H5Screate_simple(rank, dims.data(), nullptr);
   auto size      = H5Tget_size(datatype);
   int i;
   
   auto data_out = new char*[dims[0]];
   
   auto status         = H5Dread (dataset, datatype, memspace, filespace, H5P_DEFAULT, data_out);

   QStringList list;
   for (i = 0; i < dims[0]; ++i) {
      QByteArray ba(data_out[i], strlen(data_out[i])+1);
      list << QString::fromLocal8Bit(ba);
   }
   H5Dvlen_reclaim(datatype, filespace, H5P_DEFAULT, data_out);
   delete [] data_out;
   return QVariant(list);
}

bool QH5 :: readString(QString const &key)
{
   auto dataset   = H5Dopen2(d->file->id(), qPrintable(key), H5P_DEFAULT);
   auto datatype  = H5Dget_type (dataset);
   auto size      = H5Tget_size(datatype);
   QByteArray data(size+1, '\0');
   auto status  = H5Dread(dataset, datatype, H5S_ALL, H5S_ALL, H5P_DEFAULT, data.data());

   QTextStream(stdout) << data << endl;
   H5Dclose(dataset);
   return true;
}

QStringList QH5 :: datasets() 
{
   OpData od;
   herr_t status = H5Lvisit(d->file->id(), H5_INDEX_NAME, H5_ITER_NATIVE, myop, (void*)&od);
   return od.datasets;
}

bool QH5 :: close()
{
   delete d->file;
   d->file = 0;

   return true;
}

H5_END_NAMESPACE
SOFT_END_NAMESPACE
