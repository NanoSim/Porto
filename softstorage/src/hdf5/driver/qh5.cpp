#include <QtCore>
#include <vector>
#include <hdf5.h>
#include <memory>
#include "qh5.h"

SOFT_BEGIN_NAMESPACE
H5_BEGIN_NAMESPACE

template <typename Fn>
static void repeat (unsigned n, Fn fn)
{
  while (n--) fn();
}

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

static bool writeAttributeDouble (hid_t dataset, QString const &key, double const &value)
{
   auto datatype  = H5T_IEEE_F64LE;
   auto dataspace = H5Screate(H5S_SCALAR);
   auto attr = H5Acreate(dataset, qPrintable(key), datatype, dataspace, H5P_DEFAULT, H5P_DEFAULT);
   auto status = H5Awrite(attr, datatype, &value);
   H5Aclose(attr);
   H5Sclose(dataspace);
   return (status >= 0);
}

static bool writeAttribute (hid_t dataset, QString const &key, QVariant const &value)
{
   switch (value.type()) {
      case QVariant::String:
	 return writeAttributeString (dataset, key, value.toString());
      case QVariant::Int:
	 return writeAttributeInt (dataset, key, value.toInt());
      case QVariant::Double:
	 return writeAttributeDouble (dataset, key, value.toDouble());
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

static bool writeStringList(hid_t file_id, QString const &path, QStringList const &xs, QVariantMap const &attr)
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

   for (auto a : attr.keys()) {
      writeAttribute(dataset, a, attr[a]);
   }

   H5Dclose(dataset);
   H5Sclose(dataspace);
  
   return (status >= 0);
}

static bool writeInt(hid_t file_id, QString const &path, int value, QVariantMap const &attr)
{
   auto datatype  = H5T_STD_I32LE;
   auto dataspace = H5Screate(H5S_SCALAR);
   auto dataset   = H5Dcreate(file_id, qPrintable(path), datatype, dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
   auto status    = H5Dwrite(dataset, datatype, H5S_ALL, H5S_ALL, H5P_DEFAULT, &value);

   for (auto a : attr.keys()) {
      writeAttribute(dataset, a, attr[a]);
   }

   H5Dclose(dataset);
   H5Sclose(dataspace);
   return (status >= 0);
}

static bool writeDouble(hid_t file_id, QString const &path, double const & value, QVariantMap const &attr)
{
   auto datatype  = H5T_IEEE_F64LE;
   auto dataspace = H5Screate(H5S_SCALAR);
   auto dataset   = H5Dcreate(file_id, qPrintable(path), datatype, dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
   auto status    = H5Dwrite(dataset, datatype, H5S_ALL, H5S_ALL, H5P_DEFAULT, &value);

   for (auto a : attr.keys()) {
      writeAttribute(dataset, a, attr[a]);
   }

   H5Dclose(dataset);
   H5Sclose(dataspace);
   return (status >= 0); 
}

static QVector<int> getListDims(QVariant const &v)
{
   if (v.type() == QVariant::List) {
      QVector<int> dim = QVector<int>() << v.toList().count() << getListDims(v.toList().first());
      return dim;
   }
   return QVector<int>();
}

static QVariant::Type getListType(QVariant const &v)
{
   if (v.type() != QVariant::List) {
      return v.type();
   }
   return getListType (v.toList().first());
}

template <class T>
QVector<T> stdFlat (QVariant const &value) 
{
   QVector<T> vector;
   if (value.type() != QVariant::List) {
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
   if (value.type() != QVariant::List) {
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
      while (data [i]) {
	 delete [] data[i];
	 i++;
      }
      
      delete [] data;
   }
}; // class StringListData

static bool writeVariantStringList(QH5File *file, QString const &path, QVariantList const &lst, QVariantMap const &attr)
{
   auto datatype  = QH5Datatype::String;
   auto dims      = getListDims(lst);
   auto dataspace = new QH5Dataspace(dims, file);
   auto dataset   = new QH5Dataset(file, dataspace, path, datatype, dataspace); 
   QStringList input = variantListToStringList(lst);
   StringListData sd(input);
   auto result = dataset->write(sd.constData());  
   for (auto a : attr.keys()) {
      writeAttribute(dataset->id(), a, attr[a]);
   }

   return result;
}

static bool writeVariantIntList(QH5File *file, QString const &path, QVariantList const &list, QVariantMap const &attr)
{
   auto datatype  = QH5Datatype::Int32;
   auto dims      = getListDims(list);
   auto dataspace = new QH5Dataspace(dims);
   auto dataset   = new QH5Dataset(file, dataspace, path, datatype, dataspace); 
   auto qvec      = stdFlat<int>(list);
   auto ptr       = qvec.data();
   auto status    = dataset->write(ptr);
   for (auto a : attr.keys()) {
      writeAttribute(dataset->id(), a, attr[a]);
   }
  
   delete dataspace;
   return status;
}

static bool writeVariantDoubleList(QH5File *file, QString const &path, QVariantList const &list, QVariantMap const &attr)
{
   auto datatype  = QH5Datatype::Double;
   auto dims      = getListDims(list);
   auto dataspace = new QH5Dataspace(dims);
   auto dataset   = new QH5Dataset(file, dataspace, path, datatype, dataspace); 
   auto qvec      = stdFlat<double>(list);
   auto ptr       = qvec.data();
   auto status    = dataset->write(ptr);
   for (auto a : attr.keys()) {
      writeAttribute(dataset->id(), a, attr[a]);
   }
  
   delete dataspace;
   return status;
}

static bool writeVariantList(QH5File *file, QString const &path, QVariantList const &list, QVariantMap const &attr)
{
   auto type = getListType(list);
   switch (type) {
     case QVariant::String:
	return writeVariantStringList(file, path, list, attr);
	 break;
      case QVariant::Int:
	 return writeVariantIntList(file, path, list, attr);
	 break;
      case QVariant::Double:
	 return writeVariantDoubleList(file, path, list, attr);
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
      case QVariant::String:
	 return writeString(d->file->id(), path, value.toString(), attributes);
      case QVariant::Int:
	 return writeInt(d->file->id(), path, value.toInt(), attributes);
      case QVariant::Double:
	 return writeDouble(d->file->id(), path, value.toDouble(), attributes);
      case QVariant::StringList:
	 return writeStringList(d->file->id(), path, value.toStringList(), attributes);  
      case QVariant::List:
	 return writeVariantList(d->file, path, value.toList(), attributes);
      default:
	 QTextStream(stderr) << "Undefined type";
	 return false;
   }
}

bool QH5 :: describeType (QVariant const &v)
{
   QTextStream(stdout) << v.typeName();
   if (v.type() == QVariant::List) {
      QTextStream(stdout) << "[" << v.toList().count() << "] ";
      return describeType (v.toList().first());
   }
   QTextStream(stdout) << "\n";
   return true;
}



static herr_t op (hid_t /*loc_id*/, const char *name, const H5O_info_t *info,
		  void */*op_data*/)
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
   }
   return 0;
}

static herr_t op_L (hid_t loc_id, const char *name, const H5L_info_t */*info*/,
		    void */*op_data*/)
{
   H5O_info_t infobuf;
   auto status = H5Oget_info_by_name (loc_id, name, &infobuf, H5P_DEFAULT);
   Q_UNUSED(status);
   return op (loc_id, name, &infobuf, nullptr);
}

bool QH5 :: info()
{
   auto status = H5Ovisit(d->file->id(), H5_INDEX_NAME, H5_ITER_NATIVE,
			  op, nullptr);

   status = H5Lvisit (d->file->id(), H5_INDEX_NAME, H5_ITER_NATIVE, op_L, nullptr);
   return (status <= 0);
}

struct OpData {
   QStringList datasets;
};

herr_t myop(hid_t loc_id, const char *name, const H5L_info_t */*info*/,void *op_data)
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

template <class T>
T* allocateBuffer(QVector<hsize_t> const &dims) {  
  auto totsize   = ([dims]() -> int {
      int acc = 1;
      for (auto v : dims) {
	acc *= v;
      }
      return acc;
    })();
  
  return (new T[totsize]);
}

template <class T>
static QVariant reshape (QVector<hsize_t> const &dims, T *data, int &d)
{
  QVariantList list;
  if (dims.length() == 0) {
    return QVariant::fromValue(data[d++]);
  }
  
  if (dims.length() == 1) {
    repeat (dims[0], [&](){
	list << QVariant::fromValue(data[d++]);
      });
    return list;
  }

  repeat (dims[0], [&](){
      list << reshape(dims.mid(1), data, d);
    });
  
  return list;
}

template <class T>
static QVariant reshape (QVector<hsize_t> const &dims, T *data)
{
  int v = 0;
  return reshape<T>(dims, data, v);
}

static QVariant reshapeStr(QVector<hsize_t> const &dims, char **data, int &d)
{
  Q_ASSERT(dims.length() != 0);

  QVariantList list;
  if (dims.length() == 1) {
    repeat (dims[0], [&](){
	list << QString::fromLatin1(data[d++]);
      });
    return list;
  }

  repeat (dims[0], [&](){
      list << reshapeStr(dims.mid(1), data, d);
    });

  return list;
}

static QVariant reshapeStr(QVector<hsize_t> const &dims, char **data)
{
  int v = 0;
  return reshapeStr(dims, data, v);
}

QVariant QH5 :: read (QString const &key)
{
  auto dataset   = H5Dopen2 (d->file->id(), qPrintable (key), H5P_DEFAULT);
  auto datatype  = H5Dget_type (dataset);
  auto dclass    = H5Tget_class (datatype);
  auto dataspace = H5Dget_space (dataset);
  auto rank      = H5Sget_simple_extent_ndims (dataspace);
  auto size      = H5Tget_size(datatype);
  QVector<hsize_t> dims(rank);
  auto status    = H5Sget_simple_extent_dims(dataspace, dims.data(), nullptr); 
  auto memspace  = H5Screate_simple(rank, dims.data(), nullptr);

  if (dclass == H5T_STRING && rank == 0) {
    QByteArray strbuff(size+1, '\0');
    status = H5Dread(dataset, datatype, memspace, dataspace, H5P_DEFAULT, strbuff.data());
    return QVariant(strbuff);
  }
  
  void *data= nullptr;
  switch (dclass) {
  case H5T_INTEGER:
    data = allocateBuffer<qint32>(dims);    
    break;
  case H5T_FLOAT:
    data = allocateBuffer<double>(dims);
    break;
  case H5T_STRING:
    data = allocateBuffer<char*>(dims);
    break;
  default:
    break;
  }
  
  status = H5Dread(dataset, datatype, memspace, dataspace, H5P_DEFAULT, data);
  switch (dclass) {
  case H5T_INTEGER:
    return reshape(dims, (qint32*)data);
    break;
  case H5T_FLOAT:
    return reshape(dims, (double*)data);
    break;
  case H5T_STRING:
    return reshapeStr(dims, (char**)data);
    break;
  default:
    break;
  }
  
  qDebug() << "status= " << status;
  return QVariant(true);
}

QStringList QH5 :: datasets() 
{
   OpData od;
   herr_t status = H5Lvisit(d->file->id(), H5_INDEX_NAME, H5_ITER_NATIVE, myop, (void*)&od);
   Q_UNUSED(status);
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
