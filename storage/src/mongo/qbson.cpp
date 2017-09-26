#include <QVector>
#include <QList>
#include <QStringList>
#include <list>
#include <string>
#include <bson.h>
#include "qbson.h"

SOFT_BEGIN_NAMESPACE
BSON_BEGIN_NAMESPACE

static QStringList fromStdStringList(StdStringList const &xs)
{
  QVector<QString> ys(xs.size());
  std::transform(xs.begin(), xs.end(), ys.begin(), &QString::fromStdString);
  return QList<QString>::fromVector(ys);
}


static StdStringList fromQStringList(QStringList const &ys)
{
  QVector<QString> vs = QVector<QString>::fromList(ys);
  StdStringList xs(vs.size());
  std::transform(vs.begin(), vs.end(), xs.begin(), [](QString const &str){return str.toStdString();});
  return xs;
}

static void bsonDeleter (bson_t *ptr)
{
   bson_destroy (ptr);
}

Bson ::Bson (QString const& json, QObject * parent)
  : QObject(parent)
    , bson (bson_new(), bsonDeleter)
{
   bson_error_t error;
   bson_init_from_json(bson.get(), qPrintable(json), json.length(), &error);
}

Bson :: Bson (QObject * parent)
   : QObject (parent)
    , bson (bson_new(), bsonDeleter)
{}

Bson :: Bson (bson_t *bson, QObject * parent)
   : QObject (parent)
   , bson (bson, bsonDeleter)
{}

Bson :: Bson (Bson const & other)
   : QObject (other.parent())
   , bson (other.bson)
{}

Bson :: Bson (Bson && other)
   : QObject (std::move (other.parent()))
   , bson (std::move (other.bson))
{}

Bson :: ~Bson()
{}

bool Bson :: appendBson (char const *key, Bson const &value)
{
  auto isOk = bson_append_document (bson.get(),
				    key,
				    strlen(key),
				    (const bson_t*)value.data());
  return isOk;
}

bool Bson :: appendString (char const * key, char const * value)
{
   auto isOk = bson_append_utf8 (bson.get(),
                                key,
                                strlen(key),
                                value,
                                strlen(value));
   return isOk;
}

bool Bson :: append (const char *key, soft::StdString const &value)
{
  appendString(key, value.c_str());
  return true;
}

bool Bson :: append (const char *key, Bson const &value)
{
  return appendBson(key, value);
}

bool Bson :: append(char const *key, soft::StdInt const &value)
{
  return appendInt32(key, value);
}

bool Bson :: append(char const *key, soft::StdDouble const &value)
{
  return appendDouble(key, value);
}

bool Bson :: appendStringList (char const * key, QStringList const &value)
{
  bson_t b;
  uint idx = 0;
  bson_append_array_begin(bson.get(), key, strlen(key), &b);
  for (auto v : value) {
    auto utf8 = v.toUtf8();
    bson_append_utf8(&b, qPrintable(QString::number(idx++)), -1, utf8.constData(), utf8.size());
  }
  auto isOk = bson_append_array_end(bson.get(), &b);

  return isOk;
}

bool Bson :: appendInt32 (char const * key, qint32 value)
{
   auto isOk = bson_append_int32 (bson.get(),
                                  key,
                                  strlen(key),
                                  value);
   return isOk;
}

bool Bson :: appendInt64 (char const * key, qint64 value)
{
   auto isOk = bson_append_int64 (bson.get(),
                                  key,
                                  strlen(key),
                                  value);
   return isOk;
}

bool Bson :: appendBool (char const * key, bool value)
{
   auto isOk = bson_append_bool (bson.get(),
                                  key,
                                  strlen(key),
                                  value);
   return isOk;
}

bool Bson :: getBool (const char *key, bool &value) const
{
  bson_iter_t iter;
  if (bson_iter_init_find(&iter, bson.get(), key) &&
      BSON_ITER_HOLDS_BOOL(&iter)) {
    auto v = bson_iter_value(&iter);
    value = v->value.v_bool;
    return true;
  }
  return false;
}

bool Bson :: appendDouble(char const * key, double const &value)
{
  auto isOk = BSON_APPEND_DOUBLE(bson.get(), key, value);
  return isOk;
}

bool Bson :: appendFloat(char const * key, float const &value)
{
  auto isOk = BSON_APPEND_DOUBLE(bson.get(), key, value);
  return isOk;
}

bool Bson :: appendBinary (char const *key, QByteArray const &value)
{
  auto isOk = BSON_APPEND_BINARY(bson.get(), key, BSON_SUBTYPE_BINARY, (const uint8_t*)value.constData(), value.size());
  return isOk;
}

bool Bson :: appendIntArray(char const *key, soft::StdIntArray const &value)
{
  bson_t array;
  uint idx = 0;
  bson_append_array_begin(bson.get(), key, strlen(key), &array);
  for (auto v : value) {
    bson_append_int32(&array, qPrintable(QString::number(idx++)), -1, v);
  }
  auto isOk = bson_append_array_end(bson.get(), &array);
  return isOk;
}

bool Bson :: append(char const *key, soft::StdIntArray const &value)
{
  return appendIntArray(key, value);
}

bool Bson :: append(char const *key, soft::StdStringList const &value)
{
  return appendStringList(key, fromStdStringList(value));
}

bool Bson :: appendDoubleArray(char const *key, soft::StdDoubleArray const &value)
{
  bson_t array;
  uint idx = 0;
  bson_append_array_begin(bson.get(), key, strlen(key), &array);
  for (auto v : value) {
    bson_append_double(&array, qPrintable(QString::number(idx++)), -1, v);
  }
  auto isOk = bson_append_array_end(bson.get(), &array);
  return isOk;
}

bool Bson :: append(char const *key, soft::StdDoubleArray const &value)
{
  return appendDoubleArray(key, value);
}

bool Bson :: appendDoubleArray2D(char const *key, soft::StdDoubleArray2D const &value)
{
  bson_t array;
  bson_t inner;
  uint idx = 0;
  bson_append_array_begin(bson.get(), key, strlen(key), &array);
  for (auto v : value) {
    bson_append_array_begin(&array, qPrintable(QString::number(idx++)), -1, &inner);
    uint inner_idx = 0;
    for (auto d : v) {
      bson_append_double(&inner, qPrintable(QString::number(inner_idx++)), -1, d);
    }
    bson_append_array_end(&array, &inner);

  }
  auto isOk = bson_append_array_end(bson.get(), &array);
  return isOk;
}

bool Bson :: append(char const *key, soft::StdDoubleArray2D const &value)
{
  return appendDoubleArray2D(key, value);
}

bool Bson :: appendDoubleArray3D(char const *key, soft::StdDoubleArray3D const &value)
{
  bson_t array;
  bson_t inner;
  bson_t inner2;
  uint idx = 0;
  bson_append_array_begin(bson.get(), key, strlen(key), &array);
  for (auto v : value) {
    bson_append_array_begin(&array, qPrintable(QString::number(idx++)), -1, &inner);
    uint inner_idx = 0;
    for (auto d : v) {
      bson_append_array_begin(&inner, qPrintable(QString::number(inner_idx++)), -1, &inner2);
      uint inner_idx2 = 0;
      for (auto x: d) {
	bson_append_double(&inner2, qPrintable(QString::number(inner_idx2++)), -1, x);
      }
      bson_append_array_end(&inner, &inner2);

    }
    bson_append_array_end(&array, &inner);

  }
  auto isOk = bson_append_array_end(bson.get(), &array);
  return isOk;
}

bool Bson :: append(char const *key, soft::StdDoubleArray3D const &value)
{
  return appendDoubleArray3D(key, value);
}


bool Bson :: getBinary (char const *key, QByteArray &value) const
{
  bson_iter_t iter;
  if (bson_iter_init_find(&iter, bson.get(), key) &&
      BSON_ITER_HOLDS_BINARY(&iter)) {
    auto v = bson_iter_value(&iter);
    value = QByteArray::fromRawData((const char*)v->value.v_binary.data, v->value.v_binary.data_len);
    return true;
  }
  return false;
}

bool Bson :: getInt32(char const * key, qint32 &value) const
{
  bson_iter_t iter;
  if (bson_iter_init_find(&iter, bson.get(), key) &&
      BSON_ITER_HOLDS_INT32(&iter)) {
    auto v = bson_iter_value(&iter);
    value = v->value.v_int32;
    return true;
  }
  return false;
}

bool Bson :: get(const char *key, soft::StdInt &value) const
{
  return getInt32(key, value);
}

bool Bson :: get(char const *key, soft::StdDouble &value) const
{
  return getDouble(key, value);
}

bool Bson :: get(const char *key, soft::StdString &value) const
{
  QString temp;
  auto ret = getString(key, temp);
  value = temp.toStdString();
  return ret;
}

bool Bson :: get(char const *key, soft::StdStringList &value) const
{
  QStringList v;
  auto ret = getStringList(key, v);
  value = fromQStringList(v);
  return ret;
}


bool Bson :: getString(char const * key, QString &value) const
{
  bson_iter_t iter;
  if (bson_iter_init_find(&iter, bson.get(), key) &&
      BSON_ITER_HOLDS_UTF8(&iter)) {
    auto v = bson_iter_value(&iter);
    QByteArray ba(v->value.v_utf8.str, v->value.v_utf8.len);
    value = QString::fromUtf8(ba);
    return true;
  }
  return false;
}

bool Bson :: getStringList (char const * key, QStringList &value) const
{
  bson_iter_t iter;
  bson_iter_t subiter;
  value.clear();
  if (bson_iter_init_find(&iter, bson.get(), key) &&
      BSON_ITER_HOLDS_ARRAY(&iter) &&
      bson_iter_recurse(&iter, &subiter)) {
    while (bson_iter_next(&subiter) &&
	   BSON_ITER_HOLDS_UTF8(&subiter)) {
      auto v = bson_iter_value(&subiter);
      QByteArray ba(v->value.v_utf8.str, v->value.v_utf8.len);
      value << QString::fromUtf8(ba);
    }
    return true;
  }
  return false;
}

bool Bson :: getIntArray(char const *key, soft::StdIntArray &value) const
{
  return get(key, value);
}

bool Bson :: getDoubleArray(char const *key, soft::StdDoubleArray &value) const
{
  return get(key, value);
}

bool Bson :: getDoubleArray2D(char const *key, soft::StdDoubleArray2D &value) const
{
  return get(key, value);
}

bool Bson :: getDoubleArray3D(char const *key, soft::StdDoubleArray3D &value) const
{
  return get(key, value);
}

bool Bson :: get(char const *key, soft::StdIntArray &value) const
{
  bson_iter_t iter;
  bson_iter_t subiter;
  value.clear();
  if (bson_iter_init_find(&iter, bson.get(), key) &&
      BSON_ITER_HOLDS_ARRAY(&iter) &&
      bson_iter_recurse(&iter, &subiter)) {
    while (bson_iter_next(&subiter) &&
	   BSON_ITER_HOLDS_INT32(&subiter)) {
      auto v = bson_iter_value(&subiter);
      value.push_back(v->value.v_int32);
    }
    return true;
  }
  return false;
}

bool Bson :: get(char const *key, soft::StdDoubleArray &value) const
{
  bson_iter_t iter;
  bson_iter_t subiter;
  value.clear();
  if (bson_iter_init_find(&iter, bson.get(), key) &&
      BSON_ITER_HOLDS_ARRAY(&iter) &&
      bson_iter_recurse(&iter, &subiter)) {
    while (bson_iter_next(&subiter) &&
	   BSON_ITER_HOLDS_DOUBLE(&subiter)) {
      auto v = bson_iter_value(&subiter);
      value.push_back(v->value.v_double);
    }
    return true;
  }
  return false;
}

bool Bson :: get(char const *key, soft::StdDoubleArray2D &value) const
{
  bson_iter_t iter;
  bson_iter_t subiter;
  bson_iter_t subsubiter;
  value.clear();
  if (bson_iter_init_find(&iter, bson.get(), key) &&
      BSON_ITER_HOLDS_ARRAY(&iter) &&
      bson_iter_recurse(&iter, &subiter)) {
    while (bson_iter_next(&subiter) &&
	   BSON_ITER_HOLDS_ARRAY(&subiter) &&
	   bson_iter_recurse(&subiter, &subsubiter)) {
      StdDoubleArray inner;
      while (bson_iter_next(&subsubiter) &&
	     BSON_ITER_HOLDS_DOUBLE(&subsubiter)) {
	auto v = bson_iter_value(&subsubiter);
	inner.push_back(v->value.v_double);
      }
      value.push_back(inner);
    }
    return true;
  }
  return false;
}

Bson Bson :: getBson(char const *key) const
{
  bson_iter_t iter;

  if (bson_iter_init_find(&iter, bson.get(), key) &&
      BSON_ITER_HOLDS_DOCUMENT(&iter)) {
    const uint8_t *data = NULL;
    uint32_t len = 0;
    bson_iter_document(&iter, &len, &data);
    bson_t *bson = bson_new_from_data(data, len);
    return Bson(bson);
  }
  return Bson();
}

bool Bson :: get(char const *key, soft::StdDoubleArray3D &value) const
{
  bson_iter_t iter;
  bson_iter_t subiter;
  bson_iter_t subsubiter;
  bson_iter_t subsubsubiter;
  value.clear();
  if (bson_iter_init_find(&iter, bson.get(), key) &&
      BSON_ITER_HOLDS_ARRAY(&iter) &&
      bson_iter_recurse(&iter, &subiter)) {
    while (bson_iter_next(&subiter) &&
	   BSON_ITER_HOLDS_ARRAY(&subiter) &&
	   bson_iter_recurse(&subiter, &subsubiter)) {
      StdDoubleArray2D inner;
      while(bson_iter_next(&subsubiter) &&
	   BSON_ITER_HOLDS_ARRAY(&subsubiter) &&
	    bson_iter_recurse(&subsubiter, &subsubsubiter)) {
	StdDoubleArray innerinner;
	while (bson_iter_next(&subsubsubiter) &&
	       BSON_ITER_HOLDS_DOUBLE(&subsubsubiter)) {
	  auto v = bson_iter_value(&subsubsubiter);
	  innerinner.push_back(v->value.v_double);
	}
	inner.push_back(innerinner);
      }
      value.push_back(inner);
    }
    return true;
  }
  return false;
}

bool Bson :: getInt64(char const * key, qint64 &value) const
{
  bson_iter_t iter;
  if (bson_iter_init_find(&iter, bson.get(), key) &&
      BSON_ITER_HOLDS_INT64(&iter)) {
    auto v = bson_iter_value(&iter);
    value = v->value.v_int64;
    return true;
  }
  return false;
}

bool Bson :: getDouble(char const *key, double &value) const
{
  bson_iter_t iter;
  if (bson_iter_init_find(&iter, bson.get(), key) &&
      BSON_ITER_HOLDS_DOUBLE(&iter)) {
    auto v = bson_iter_value(&iter);
    value = v->value.v_double;
    return true;
  }
  return false;
}

bool Bson :: getFloat(char const *key, float &value) const
{
  bson_iter_t iter;
  if (bson_iter_init_find(&iter, bson.get(), key) &&
      BSON_ITER_HOLDS_DOUBLE(&iter)) {
    auto v = bson_iter_value(&iter);
    value = (float)v->value.v_double;
    return true;
  }
  return false;
}

QStringList Bson :: keys() const
{
  QStringList retval;
  bson_iter_t iter;
  if (bson_iter_init(&iter, bson.get())) {
    while (bson_iter_next(&iter)) {
      retval << QString::fromLocal8Bit(bson_iter_key(&iter));
    }
  }
  return retval;
}

qint32 Bson :: countKeys() const
{
   return bson_count_keys(bson.get());
}

QString Bson :: asString() const
{
   char* str = bson_as_json(bson.get(), nullptr);
   QString ret = QString::fromLatin1(str);
   bson_free(str);
   return ret;
}

bool Bson :: hasField(char const *key) const
{
  return bson_has_field(bson.get(), key);
}

bson_t* Bson :: data() const
{
   return bson.get();
}

Bson& Bson :: operator=(Bson const &other)
{
  bson = other.bson;
  return *this;
}

std::shared_ptr<struct _bson_t> Bson :: bsonPtr()
{
  return bson;
}

BSON_END_NAMESPACE
SOFT_END_NAMESPACE
