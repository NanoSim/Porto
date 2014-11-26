#include <QtCore>
#include <vector>
#include <hdf5.h>
#include "qh5.h"


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
   qDebug() << "writing string" << xs;
   auto datatype = H5Tcopy(H5T_C_S1);
   H5Tset_size (datatype, H5T_VARIABLE);

   std::vector<hsize_t> sizes(1);
   sizes[0] = xs.size();

   auto dataspace = H5Screate_simple(1, &sizes[0], nullptr);
   auto dataset = H5Dcreate(file_id, qPrintable(path), datatype, dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

   char *output[xs.size()];
   int i = 0;
   for (auto x : xs) {
      auto l8 = x.toLocal8Bit();
      output[i] = new char[l8.size()];
      strncpy(output[i], l8.constData(), l8.size());
      ++i;
   }
   auto status = H5Dwrite(dataset, datatype, H5S_ALL, H5S_ALL, H5P_DEFAULT, output);

   for (i = 0; i < xs.size(); ++i) {
      delete [] output[i];
   }
   H5Dclose(dataset);
   H5Sclose(dataspace);
  
   return (status >= 0);
}

static bool writeInt(hid_t file_id, QString const &path, int value)
{
   qDebug() << "write int" << value;
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
   qDebug() << "write double" << value;
   auto datatype  = H5T_IEEE_F64LE;
   auto dataspace = H5Screate(H5S_SCALAR);
   auto dataset   = H5Dcreate(file_id, qPrintable(path), datatype, dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
   auto status    = H5Dwrite(dataset, datatype, H5S_ALL, H5S_ALL, H5P_DEFAULT, &value);
   H5Dclose(dataset);
   H5Sclose(dataspace);
   return (status >= 0); 
}


class QH5::Private {
   friend class QH5;

   Private ()
      : file (nullptr)
   {}

   QH5File *file;
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
      default:
	 qDebug() << "Undefined type";
	 return false;
   }
}
