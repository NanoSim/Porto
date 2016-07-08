#include <bson.h>
#include <QString>
#include <QByteArray>
#include "bsonmodel.h"

SOFT_BEGIN_NAMESPACE

class BsonModel :: Private
{
   friend class BsonModel;

   Private()
      : bson (bson_new ())
   {}

   ~Private()
   {
      bson_destroy(bson);
   }
   bson_t *bson;
};

BsonModel :: BsonModel()
   : IDataModel()
   , d(new BsonModel::Private)

{}

BsonModel :: ~BsonModel()
{
   delete d;
}

IDataModel* BsonModel :: createModel()
{
   return new BsonModel;
}

bool BsonModel :: appendDimension  (const char *, StdUInt)
{
  return false;
}


bool BsonModel :: appendVariant (const char *, StdVariant const &)
{
  return false;
}

bool BsonModel :: appendInt8(const char *key, int8_t value)
{
  return BSON_APPEND_INT32(d->bson, key, (int32_t)value);
}

bool BsonModel :: appendInt16(const char *key, int16_t value)
{
  return BSON_APPEND_INT32(d->bson, key, (int32_t)value);
}

bool BsonModel :: appendInt32(const char *key, int32_t value)
{
  return BSON_APPEND_INT32(d->bson, key, value);
}

bool BsonModel :: appendInt64(const char *key, int64_t value)
{
  return BSON_APPEND_INT64(d->bson, key, value);
}

bool BsonModel :: appendUInt8(const char *key, uint8_t value)
{
  return BSON_APPEND_INT32(d->bson, key, (int32_t)value);
}

bool BsonModel :: appendUInt16(const char *key, uint16_t value)
{
  return BSON_APPEND_INT32(d->bson, key, (int32_t)value);
}

bool BsonModel :: appendUInt32(const char *key, uint32_t value)
{
  return BSON_APPEND_INT32(d->bson, key, value);
}

bool BsonModel :: appendUInt64(const char *key, uint64_t value)
{
  return BSON_APPEND_INT64(d->bson, key, (int64_t)value);
}

bool BsonModel :: appendFloat(const char *key, float value)
{
  return BSON_APPEND_DOUBLE(d->bson, key, value);
}

bool BsonModel :: appendDouble(const char *key, double value)
{
   return BSON_APPEND_DOUBLE(d->bson, key, value);
}

bool BsonModel :: appendBool(const char *key, bool value)
{
   return BSON_APPEND_BOOL(d->bson, key, value);
}

bool BsonModel :: appendArray(const char *key, const IDataModel *model)
{
   auto m = dynamic_cast<const BsonModel*>(model);
   return bson_append_array (d->bson, key, strlen(key), m->d->bson);
}

bool BsonModel :: appendModel(const char *key, IDataModel *model)
{
   auto m = dynamic_cast<const BsonModel*>(model);
   return bson_append_document (d->bson, key, strlen(key), m->d->bson);
}

bool BsonModel :: appendInt32Array (const char *key, const std::vector<int32_t> &value)
{
  auto b = bson_new();
  auto it = value.cbegin();
  if (it != value.cend()) {
    BSON_APPEND_INT32(b, "0", *it++);
  }
  while (it != value.cend()) {
    BSON_APPEND_INT32(b, "_id", *it++);
  }

  return bson_append_array(d->bson, key, strlen(key), b);
}

bool BsonModel :: appendDoubleArray (const char *key, const std::vector<double> &value)
{
  auto b = bson_new();
  auto it = value.cbegin();
  if (it != value.cend()) {
    BSON_APPEND_DOUBLE(b, "0", *it++);
  }
  while (it != value.cend()) {
    BSON_APPEND_DOUBLE(b, "_id", *it++);
  }

  return bson_append_array(d->bson, key, strlen(key), b);
}

bool BsonModel :: appendStringArray (const char *key, const std::vector<std::string> &value)
{
  auto b = bson_new();
  auto it = value.cbegin();
  if (it != value.cend()) {
    bson_append_utf8(b, "0", 1, (*it).c_str(), (*it).length());
    ++it;
  }
  while (it != value.cend()) {
    bson_append_utf8(b, "_id", 3, (*it).c_str(), (*it).length());
    ++it;
  }

  return bson_append_array(d->bson, key, strlen(key), b);
}


bool BsonModel :: appendByteArray(const char* key, const std::vector<unsigned char> &value)
{
  return BSON_APPEND_BINARY(d->bson, key, BSON_SUBTYPE_BINARY, (unsigned char*)value.data(), value.size());
}

bool BsonModel :: appendString(const char *key, const std::string &str)
{
  QByteArray utfStr = QString::fromStdString(str).toUtf8();
  bson_append_utf8 (d->bson, key, strlen(key), utfStr.constData(), utfStr.length());
}

bool BsonModel :: getDimension (const char *, StdUInt &) const
{
  return false;
}

bool BsonModel :: getVariant (const char *, StdVariant &) const
{
  return false;
}

bool BsonModel :: getString (const char *, std::string &str) const
{
  return false;
}

bool BsonModel :: getInt8 (const char *, int8_t &) const
{
  return false;
}

bool BsonModel :: getUInt8 (const char *, uint8_t &) const
{
  return false;
}

bool BsonModel :: getInt16 (const char *, int16_t &) const
{
  return false;
}

bool BsonModel :: getUInt16 (const char *, uint16_t &) const
{
  return false;
}

bool BsonModel :: getInt32 (const char *, int32_t &) const
{
  return false;
}

bool BsonModel :: getUInt32 (const char *, uint32_t &) const
{
  return false;
}

bool BsonModel :: getInt64 (const char *, int64_t &) const
{
  return false;
}

bool BsonModel :: getUInt64 (const char *, uint64_t &) const
{
  return false;
}

bool BsonModel :: getFloat (const char *, float &) const
{
  return false;
}

bool BsonModel :: getDouble (const char *, double &) const
{
  return false;
}

bool BsonModel :: getBool (const char *, bool &) const
{
  return false;
}

bool BsonModel :: getInt32Array (const char *, std::vector<int32_t> &) const
{
  return false;
}

bool BsonModel :: getDoubleArray (const char *, std::vector<double> &) const
{
  return false;
}

bool BsonModel :: getByteArray (const char *, std::vector<unsigned char> &) const
{
  return false;
}

bool BsonModel :: getStringArray   (const char *, std::vector<std::string> &) const
{
  return false;
}

bool BsonModel :: getArray (const char *, IDataModel *) const
{
  return false;
}

IDataModel* BsonModel :: getModel (const char *) const
{
  return false;
}

const _bson_t *BsonModel :: bson() const
{
   return (_bson_t const*) d->bson;
}


bool BsonModel :: appendDoubleArray2D (const char *, const std::vector<std::vector<double> >&)
{
  return false;
}

bool BsonModel :: appendDoubleArray3D (const char *, const std::vector<std::vector<std::vector<double> > >&)
{
  return false;
}

bool BsonModel :: getDoubleArray2D (const char *, std::vector<std::vector<double> > &) const
{
  return false;
}

bool BsonModel :: getDoubleArray3D (const char *, std::vector<std::vector<std::vector<double> > > &) const
{
  return false;
}


SOFT_END_NAMESPACE
