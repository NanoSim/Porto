#ifndef QBSON_H_DEF
#define QBSON_H_DEF

#include <QObject>
#include <memory>
#include <mongoc.h>
#include "mongons.h"
#include <bson.h>

SOFT_BEGIN_NAMESPACE

MONGO_BEGIN_NAMESPACE
class Client;
class Collection;
class GridFSFileOpt;
MONGO_END_NAMESPACE

BSON_BEGIN_NAMESPACE

class Bson : public QObject
{
   Q_OBJECT

public:
   explicit Bson (QObject * parent = nullptr);
   Bson (QString const& json, QObject * parent = nullptr);
   Bson (bson_t *bsn, QObject * parent = nullptr);
   Bson (Bson const & other);
   Bson (Bson && other);
   virtual ~Bson();

   bool appendString (char const * key, char const * value);
   bool appendInt32 (char const * key, qint32 value);
   bool appendInt64 (char const * key, qint64 value);
   bool appendBool (char const * key, bool value);
   qint32 countKeys() const;
   QString asString() const;

protected:
  bson_t* data() const;

private:
  friend class SOFT_NAMESPACE::MONGO_NAMESPACE::GridFSFileOpt;
  friend class SOFT_NAMESPACE::MONGO_NAMESPACE::Client;
  friend class SOFT_NAMESPACE::MONGO_NAMESPACE::Collection;
  std::shared_ptr<bson_t> bson;
};

BSON_END_NAMESPACE
SOFT_END_NAMESPACE

#endif // QBSON_H_DEF
