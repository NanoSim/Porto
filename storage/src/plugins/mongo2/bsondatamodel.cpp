#include <stdexcept>
#include <cstring>
#include <algorithm>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QList>
#include "bsondatamodel.h"

SOFT_BEGIN_NAMESPACE

#define NOT_IMPLEMENTED throw std::runtime_error("Not implemented");

struct BsonDataModel :: Private
{  
  using DataModelMap = QMap<QString, IDataModel *>;

  DataModelMap dataModelMap;
};

StdStringList BsonDataModel :: getModelLabels() const
{
  StdStringList ret(d->dataModelMap.size());
  auto keys = d->dataModelMap.keys();
  std::transform(keys.begin(), keys.end(), ret.begin(),
		 [](QString &key) -> std::string {
		   return key.toStdString();
		 });
  
  return ret;
}

BsonDataModel :: BsonDataModel()
  : IDataModel()
  , d (new BsonDataModel :: Private())
{}

BsonDataModel :: ~BsonDataModel() 
{}

IDataModel* BsonDataModel ::createModel() 
{
  return new BsonDataModel;
}

bool BsonDataModel :: appendDimension (const char *key, StdUInt value) 
{
  return dimsObject.append(key, (StdInt)value);
}

bool BsonDataModel :: appendVariant (const char *key, StdVariant const &value) 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: appendString (const char*key, const StdString &value) 
{
  return propertyObject.append(key, value);
}

bool BsonDataModel :: appendInt8 (const char *key, StdInt8 value) 
{
  return propertyObject.append(key, (StdInt)value);
}

bool BsonDataModel :: appendUInt8 (const char *key, StdUInt8 value) 
{
  return propertyObject.append(key, (StdInt)value);
}

bool BsonDataModel :: appendInt16 (const char *key, StdInt16 value) 
{
  return propertyObject.append(key, (StdInt)value);
}

bool BsonDataModel :: appendUInt16 (const char *key, StdUInt16 value) 
{
  return propertyObject.append(key, (StdInt)value);
}

bool BsonDataModel :: appendInt32 (const char *key, StdInt value) 
{
  return propertyObject.append(key, value);
}

bool BsonDataModel :: appendUInt32 (const char *key, StdUInt value) 
{
  return propertyObject.append(key, (StdInt)value);
}

bool BsonDataModel :: appendInt64 (const char *key, StdInt64 value) 
{
  return propertyObject.appendInt64(key, value);
}

bool BsonDataModel :: appendUInt64 (const char *key, StdUInt64 value) 
{  
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: appendFloat (const char *key, StdFloat value) 
{
  return propertyObject.appendFloat(key, value);
}

bool BsonDataModel :: appendDouble (const char *key, StdDouble value) 
{
  return propertyObject.appendDouble(key, value);
}

bool BsonDataModel :: appendBool (const char *key, StdBool value) 
{
  // @TODO It appears that BSON objects cannot contain bools, and that the mongo insert fails if it does
  return propertyObject.appendBool(key, value);
}

bool BsonDataModel :: appendInt32Array (const char *key, StdIntArray const &value) 
{
  return propertyObject.appendIntArray(key, value);
}

bool BsonDataModel :: appendDoubleArray (const char *key, StdDoubleArray const &value) 
{
  return propertyObject.append(key, value);
}

bool BsonDataModel :: appendDoubleArray2D (const char *key, StdDoubleArray2D const&value) 
{
  return propertyObject.append(key, value);
}

bool BsonDataModel :: appendDoubleArray3D (const char *key, StdDoubleArray3D const&value) 
{
  return propertyObject.append(key, value);
}

bool BsonDataModel :: appendByteArray (const char *key, StdBlob const &value) 
{
  QByteArray buffer((const char*)value.data(), value.size());
  return propertyObject.appendBinary(key, buffer);
}

bool BsonDataModel :: appendStringArray (const char *key, StdStringList const &value) 
{
  return propertyObject.append(key, value);
}

bool BsonDataModel :: appendArray (const char *key, IDataModel const * value) 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: getDimension (const char *key, StdUInt &value) const 
{
  return propertyObject.get(key, reinterpret_cast<StdInt&>(value));
}

bool BsonDataModel :: getVariant (const char *key, StdVariant &value) const 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: getString (const char *key, StdString &value) const 
{
  return propertyObject.get(key, value);
}

bool BsonDataModel :: getInt8 (const char *key, StdInt8 &value) const 
{
  return propertyObject.getInt32(key, (StdInt&)value);
}

bool BsonDataModel :: getUInt8 (const char *key, StdUInt8 &value) const 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: getInt16 (const char *key, StdInt16 &value) const 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: getUInt16 (const char *key, StdUInt16 &value) const 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: getInt32 (const char *key, StdInt &value) const 
{
  return propertyObject.get(key, value);
}

bool BsonDataModel :: getUInt32 (const char *key, StdUInt &value) const 
{
  return propertyObject.get(key, (StdInt&)value);
}

bool BsonDataModel :: getInt64 (const char *key, StdInt64 &value) const 
{
  return propertyObject.getInt64(key, (qint64&)value);
}

bool BsonDataModel :: getUInt64 (const char *key, StdUInt64 &value) const 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: getFloat (const char *key, StdFloat &value) const 
{
  return propertyObject.getFloat(key, value);
}

bool BsonDataModel :: getDouble (const char *key, StdDouble &value) const 
{
  return propertyObject.get(key, value);
}

bool BsonDataModel :: getBool (const char *key, StdBool &value) const 
{
  return propertyObject.getBool(key, value);
}

bool BsonDataModel :: getInt32Array (const char *key, StdIntArray &value) const 
{
  return propertyObject.get(key, value);
}

bool BsonDataModel :: getDoubleArray (const char *key, StdDoubleArray &value) const 
{
  return propertyObject.get(key, value);
}

bool BsonDataModel :: getDoubleArray2D (const char *key, StdDoubleArray2D &value) const 
{
  return propertyObject.get(key, value);
}

bool BsonDataModel :: getDoubleArray3D (const char *key, StdDoubleArray3D &value) const 
{
  return propertyObject.get(key, value);
}

static std::vector<unsigned char> toUCharVector(QByteArray const &bytes)
{
  std::vector<unsigned char> cp(bytes.size());
  std::memcpy(cp.data(), bytes.constData(), bytes.size());
  return cp;
}

bool BsonDataModel :: getByteArray (const char *key, StdBlob &value) const 
{
  QByteArray buffer;
  auto ret = propertyObject.getBinary(key, buffer);
  if (ret) {
    value = toUCharVector(buffer);
  }
  return ret;
}

bool BsonDataModel :: getStringArray (const char *key, StdStringList &value) const 
{
  return propertyObject.get(key, value);
}

bool BsonDataModel :: getArray (const char *key, IDataModel * value) const 
{
  NOT_IMPLEMENTED;
  return false;
}

IDataModel * BsonDataModel :: getModel (const char *k) const 
{
  auto key = QString::fromLocal8Bit(k);
  if (d->dataModelMap.contains(key)) {
    return d->dataModelMap.value(key);
  }
  return nullptr;
}

bool BsonDataModel :: appendModel (const char *k, IDataModel *model) 
{  
  auto key = QString::fromLocal8Bit(k);
  if (!d->dataModelMap.contains(key)) {
    d->dataModelMap.insert(key, model);
    return true;
  }
  return false;
} 

SOFT_END_NAMESPACE
