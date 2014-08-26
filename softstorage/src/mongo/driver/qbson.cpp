#include <bson.h>
#include "qbson.h"

PORTO_BEGIN_NAMESPACE
BSON_BEGIN_NAMESPACE

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

bool Bson :: appendString (char const * key, char const * value)
{
   auto isOk = bson_append_utf8 (bson.get(),
                                key,
                                strlen(key),
                                value,
                                strlen(value));
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

bson_t* Bson :: data() const
{
   return bson.get();
}

BSON_END_NAMESPACE
PORTO_END_NAMESPACE

