#include <QtCore>
#include <cstdint>
#include "jsonmodel.h"

SOFT_BEGIN_NAMESPACE

class JSONModel :: Private
{
  friend class JSONModel;
  Private()
  {}

  QJsonObject propertyObject;
  QJsonObject dimsObject;
  std::function<bool(const char *, StdUInt&)>           getDimension;
  std::function<bool(const char *, StdString&)>         getString;
  std::function<bool(const char *, StdInt8&)>           getInt8;
  std::function<bool(const char *, StdUInt8&)>          getUInt8;
  std::function<bool(const char *, StdInt16 &)>         getInt16;
  std::function<bool(const char *, StdUInt16 &)>        getUInt16;
  std::function<bool(const char *, StdInt &)>           getInt32;
  std::function<bool(const char *, StdUInt &)>          getUInt32;
  std::function<bool(const char *, StdInt64 &)>         getInt64;
  std::function<bool(const char *, StdUInt64 &)>        getUInt64;
  std::function<bool(const char *, StdFloat &)>         getFloat;
  std::function<bool(const char *, StdDouble &)>        getDouble;
  std::function<bool(const char *, bool &)>             getBool;
  std::function<bool(const char *, StdIntArray &)>      getInt32Array;
  std::function<bool(const char *, StdDoubleArray &)>   getDoubleArray;
  std::function<bool(const char *, StdDoubleArray2D &)> getDoubleArray2D;
  std::function<bool(const char *, StdDoubleArray3D &)> getDoubleArray3D;
  std::function<bool(const char *, StdBlob &)>          getByteArray;
  std::function<bool(const char *, StdStringList &)>    getStringArray;
};

JSONModel :: JSONModel()
  : IDataModel()
  , data (nullptr)
  , d (new JSONModel::Private)
{}

JSONModel :: ~JSONModel()
{
  delete d;
}

void JSONModel :: registerGetDimension(std::function<bool(const char *,StdUInt&)> &fn)
{
  d->getDimension = fn;
}

void JSONModel :: registerGetInt8Fn(std::function<bool(const char *,StdInt8&)> &fn)
{
  d->getInt8 = fn;
}

void JSONModel :: registerGetUInt8Fn(std::function<bool(const char *,StdUInt8&)> &fn)
{
  d->getUInt8 = fn;
}

void JSONModel :: registerGetStringFn(std::function<bool(const char *,StdString&)> &fn)
{
  d->getString = fn;
}

void JSONModel :: registerGetInt16Fn(std::function<bool(const char *, StdInt16 &)> &fn)
{
  d->getInt16 = fn;
}

void JSONModel :: registerGetUInt16Fn(std::function<bool(const char *, StdUInt16 &)> &fn)
{
  d->getUInt16 = fn;
}

void JSONModel :: registerGetInt32Fn(std::function<bool(const char *, StdInt &)> &fn)
{
  d->getInt32 = fn;
}

void JSONModel :: registerGetUInt32Fn(std::function<bool(const char *, StdUInt &)> &fn)
{
  d->getUInt32 = fn;
}

void JSONModel :: registerGetInt64Fn(std::function<bool(const char *, StdInt64 &)> &fn)
{
  d->getInt64 = fn;
}

void JSONModel :: registerGetUInt64Fn(std::function<bool(const char *, StdUInt64 &)> &fn)
{
  d->getUInt64 = fn;
}

void JSONModel :: registerGetFloatFn(std::function<bool(const char *, StdFloat &)> &fn)
{
  d->getFloat = fn;
}

void JSONModel :: registerGetDoubleFn(std::function<bool(const char *, StdDouble &)> &fn)
{
  d->getDouble = fn;
}

void JSONModel :: registerGetBoolFn(std::function<bool(const char *, bool &)> &fn)
{
  d->getBool = fn;
}

void JSONModel :: registerGetInt32ArrayFn(std::function<bool(const char *, StdIntArray &)> &fn)
{
  d->getInt32Array = fn;
}

void JSONModel :: registerGetDoubleArrayFn(std::function<bool(const char *, StdDoubleArray &)> &fn)
{
  d->getDoubleArray = fn;
}

void JSONModel :: registerGetDoubleArray2DFn(std::function<bool(const char *, StdDoubleArray2D &)> &fn)
{
  d->getDoubleArray2D = fn;
}

void JSONModel :: registerGetDoubleArray3DFn(std::function<bool(const char *, StdDoubleArray3D &)> &fn)
{
  d->getDoubleArray3D = fn;
}

void JSONModel :: registerGetByteArrayFn(std::function<bool(const char *, StdBlob&)> &fn)
{
  d->getByteArray = fn;
}

void JSONModel :: registerGetStringArrayFn(std::function<bool(const char *, StdStringList&)> &fn)
{
  d->getStringArray = fn;
}

IDataModel *JSONModel :: createModel()
{
  return new JSONModel;
}

bool JSONModel :: appendVariant (const char *key, StdVariant const &value)
{
  switch (value.type()) {
  case soft::StdTypes::String:
    d->propertyObject.insert(key, QJsonValue(QString::fromStdString(value.get<StdString>())));
    break;
  case soft::StdTypes::Int:
  case soft::StdTypes::UInt:
  case soft::StdTypes::Int8:
  case soft::StdTypes::UInt8:
  case soft::StdTypes::Int64:
  case soft::StdTypes::UInt64:
    d->propertyObject.insert(key, QJsonValue(value.toInt()));
    break;
  case soft::StdTypes::Double:
    d->propertyObject.insert(key, QJsonValue(value.get<StdDouble>()));
    break;
  case soft::StdTypes::DoubleArray:
    QJsonArray array;
    auto v = value.get<StdDoubleArray>();
    for (StdDoubleArray::iterator it = v.begin(); it != v.end(); ++it) {
      array.append(QJsonValue(*it));
    }
    d->propertyObject.insert(key, array);
    break;
  }
  return true;
}

bool JSONModel :: appendString     (const char* key, const StdString &value)
{
  QJsonValue jsonValue(QString::fromStdString(value));
  auto it = d->propertyObject.insert(key, jsonValue);
  bool isOk = (it != d->propertyObject.end());
  return isOk;
}

bool JSONModel :: appendInt8       (const char *key, StdInt8 value)
{
  QJsonValue jsonValue(value);
  auto it = d->propertyObject.insert(key, jsonValue);
  bool isOk = (it != d->propertyObject.end());
  return isOk;
}

bool JSONModel :: appendUInt8      (const char *key, StdUInt8 value)
{
  QJsonValue jsonValue(value);
  auto it = d->propertyObject.insert(key, jsonValue);
  bool isOk = (it != d->propertyObject.end());
  return isOk;
}

bool JSONModel :: appendInt16      (const char *key, StdInt16 value)
{
  QJsonValue jsonValue(value);
  auto it = d->propertyObject.insert(key, jsonValue);
  bool isOk = (it != d->propertyObject.end());
  return isOk;
}

bool JSONModel :: appendUInt16     (const char *key, StdUInt16 value)
{
  QJsonValue jsonValue(value);
  auto it = d->propertyObject.insert(key, jsonValue);
  bool isOk = (it != d->propertyObject.end());
  return isOk;
}

bool JSONModel :: appendInt32      (const char *key, StdInt value)
{
  QJsonValue jsonValue(value);
  auto it = d->propertyObject.insert(key, jsonValue);
  bool isOk = (it != d->propertyObject.end());
  return isOk;
}

bool JSONModel :: appendUInt32     (const char *key, StdUInt value)
{
  QJsonValue jsonValue((int)value);
  auto it = d->propertyObject.insert(key, jsonValue);
  bool isOk = (it != d->propertyObject.end());
  return isOk;
}

bool JSONModel :: appendInt64      (const char *key, StdInt64 value)
{
  QJsonValue jsonValue((int)value);
  auto it = d->propertyObject.insert(key, jsonValue);
  bool isOk = (it != d->propertyObject.end());
  return isOk;
}

bool JSONModel :: appendUInt64     (const char *key, StdUInt64 value)
{
  QJsonValue jsonValue((int)value);
  auto it = d->propertyObject.insert(key, jsonValue);
  bool isOk = (it != d->propertyObject.end());
  return isOk;
}

bool JSONModel :: appendFloat      (const char *key, StdFloat value)
{
  QJsonValue jsonValue(value);
  auto it = d->propertyObject.insert(key, jsonValue);
  bool isOk = (it != d->propertyObject.end());
  return isOk;
}

bool JSONModel :: appendDimension (const char *key, StdUInt value)
{
  QJsonValue jsonValue((int)value);
  auto it = d->dimsObject.insert(key, jsonValue);
  bool isOk = (it != d->dimsObject.end());
  return isOk;
}

bool JSONModel :: appendDouble     (const char *key, StdDouble value)
{
  QJsonValue jsonValue(value);
  auto it = d->propertyObject.insert(key, jsonValue);
  bool isOk = (it != d->propertyObject.end());
  return isOk;
}

bool JSONModel :: appendBool       (const char *key, bool value)
{
  QJsonValue jsonValue(value);
  auto it = d->propertyObject.insert(key, jsonValue);
  bool isOk = (it != d->propertyObject.end());
  return isOk;
}

bool JSONModel :: appendInt32Array (const char *key, const StdIntArray &value)
{
  QJsonArray jsonArray;
  for (auto &v: value) {
    jsonArray.append(QJsonValue(v));
  }
  auto it = d->propertyObject.insert(key, jsonArray);
  bool isOk = (it != d->propertyObject.end());
  return isOk;
}

bool JSONModel :: appendDoubleArray(const char *key, const StdDoubleArray &value)
{
  QJsonArray jsonArray;
  for (auto &v: value) {
    jsonArray.append(QJsonValue(v));
  }
  auto it = d->propertyObject.insert(key, jsonArray);
  bool isOk = (it != d->propertyObject.end());
  return isOk;
}

bool JSONModel :: appendDoubleArray2D (const char *key, const StdDoubleArray2D& value)
{
  QJsonArray jsonArray;
  for (auto &v: value) {
    QJsonArray array;
    for (auto &d: v) {
      array.append(QJsonValue(d));
    }
    jsonArray.append(array);
  }
  auto it = d->propertyObject.insert(key, jsonArray);
  bool isOk = (it != d->propertyObject.end());
  return isOk;
}

bool JSONModel :: appendDoubleArray3D (const char *key, const StdDoubleArray3D& value)
{
  QJsonArray jsonArray;
  for (auto &v: value) {
    QJsonArray aa;
    for (auto &a: v) {
      QJsonArray array;
      for (auto &d: a) {
  array.append(QJsonValue(d));
      }
      aa.append(array);
    }
    jsonArray.append(aa);
  }
  auto it = d->propertyObject.insert(key, jsonArray);
  bool isOk = (it != d->propertyObject.end());
  return isOk;
}


bool JSONModel :: appendByteArray  (const char *, const std::vector<unsigned char> &)
{
  return false;
}

bool JSONModel :: appendStringArray(const char *key, const std::vector<StdString> &value)
{
  QJsonArray jsonArray;
  for (auto &v: value) {
    jsonArray.append(QJsonValue(QString::fromStdString(v)));
  }
  auto it = d->propertyObject.insert(key, jsonArray);
  bool isOk = (it != d->propertyObject.end());
  return isOk;
}

bool JSONModel :: appendArray      (const char *, const IDataModel *)
{
  return false;
}

bool JSONModel :: appendModel      (const char *, IDataModel *)
{
  return false;
}

bool JSONModel :: getVariant (const char *, StdVariant &) const
{
  return false;
}

bool JSONModel :: getDimension (const char *key, StdUInt &value) const
{
  if (!d->getDimension) return false;
  return d->getDimension(key, value);
}

bool JSONModel :: getString (const char *key, StdString &value) const
{
  if (!d->getString) return false;
  return d->getString(key, value);
}

bool JSONModel :: getInt8          (const char *key, StdInt8 &value) const
{
  if (!d->getInt8) return false;
  return d->getInt8(key,value);
}

bool JSONModel :: getUInt8         (const char *key, StdUInt8 &value) const
{
  if (!d->getUInt8) return false;
  return d->getUInt8(key, value);
}

bool JSONModel :: getInt16         (const char *key, StdInt16 &value) const
{
  if (!d->getInt16) return false;
  return d->getInt16(key, value);
}

bool JSONModel :: getUInt16        (const char *key, StdUInt16 &value) const
{
  if (!d->getUInt16) return false;
  return d->getUInt16(key, value);
}

bool JSONModel :: getInt32         (const char *key, StdInt &value) const
{
  if (!d->getInt32) return false;
  return d->getInt32(key, value);
}

bool JSONModel :: getUInt32        (const char *key, StdUInt &value) const
{
  if (!d->getUInt32) return false;
  return d->getUInt32(key, value);
}

bool JSONModel :: getInt64         (const char *key, StdInt64 &value) const
{
  if (!d->getInt64) return false;
  return d->getInt64(key, value);
}

bool JSONModel :: getUInt64        (const char *key, StdUInt64 &value) const
{
  if (!d->getUInt64) return false;
  return d->getUInt64(key, value);
}

bool JSONModel :: getFloat         (const char *key, StdFloat &value) const
{
  if (!d->getFloat) return false;
  return d->getFloat(key, value);
}

bool JSONModel :: getDouble        (const char *key, StdDouble &value) const
{
  if (!d->getDouble) return false;
  return d->getDouble(key, value);
}

bool JSONModel :: getBool          (const char *key, bool &value) const
{
  if (!d->getBool) return false;
  return d->getBool(key, value);
}

bool JSONModel :: getInt32Array    (const char *key, StdIntArray &value) const
{
  if (!d->getInt32Array) return false;
  return d->getInt32Array(key, value);
}

bool JSONModel :: getDoubleArray   (const char *key, StdDoubleArray &value) const
{
  if (!d->getDoubleArray) return false;
  return d->getDoubleArray(key, value);
}

bool JSONModel :: getByteArray     (const char *key, StdBlob &value) const
{
  if (!d->getByteArray) return false;
  return d->getByteArray(key, value);
}

bool JSONModel :: getStringArray(const char *key, StdStringList &value) const
{
  if (!d->getStringArray) return false;
  return d->getStringArray(key, value);
}

bool JSONModel :: getArray         (const char *, IDataModel *) const
{
  return false;
}

IDataModel* JSONModel :: getModel         (const char *) const
{
  return false;
}

const QJsonObject *JSONModel :: propsJson() const
{
  return &d->propertyObject;
}

const QJsonObject *JSONModel :: dimsJson() const
{
  return &d->dimsObject;
}

void JSONModel :: setPropsJson(QJsonObject const &obj)
{
  d->propertyObject  = obj;
}

void JSONModel :: setDimsJson(QJsonObject const &obj)
{
  d->dimsObject  = obj;
}

bool JSONModel :: getDoubleArray2D (const char *key, StdDoubleArray2D &value) const
{
  if (!d->getDoubleArray2D) return false;
  return d->getDoubleArray2D(key, value);
}

bool JSONModel :: getDoubleArray3D (const char *key, StdDoubleArray3D &value) const
{
  if (!d->getDoubleArray3D) return false;
  return d->getDoubleArray3D(key, value);
}

SOFT_END_NAMESPACE
