
#include <mongoc.h>
#include <QDebug>
#include "qbson.h"
#include "qmongocursor.h"

#include "qmongocollection.h"

NAMESPACE_SOFT_BEGIN

void handleError (bson_error_t const &error)
{
  if (error.code != 0)
  {
    qWarning() << error.domain << ". " << error.code << ": " << QString::fromUtf8 (error.message);
  }
};

struct CollectionDeleter
{
  static inline void cleanup (mongoc_collection_t *ptr)
  {
    mongoc_collection_destroy (ptr);
  }
};

struct WriteConcernDeleter
{
  static inline void cleanup (mongoc_write_concern_t *ptr)
  {
    mongoc_write_concern_destroy (ptr);
  }
   
};

class QMongoCollection :: Private {
  friend class QMongoCollection;

  Private (mongoc_collection_t *coll = nullptr)
    : collection (coll)
    , write_concern (mongoc_write_concern_new()) 
  {}

  QScopedPointer <mongoc_collection_t, CollectionDeleter> collection;
  QScopedPointer <mongoc_write_concern_t, WriteConcernDeleter> write_concern;
};

QMongoCollection :: QMongoCollection (QObject * parent)
  : QObject (parent)
  , d (new QMongoCollection::Private())
{}

QMongoCollection :: QMongoCollection (_mongoc_collection_t *collection, QObject * parent)
  : QObject (parent)
  , d (new QMongoCollection::Private(collection))
{}

QMongoCollection :: ~QMongoCollection()
{
  delete d;
}

QMongoCursor *QMongoCollection :: find(QBson const &query)
{
  auto flags = MONGOC_QUERY_NONE;
  auto cursor = mongoc_collection_find (this->collection(),
					flags,
					0,
					0,
					0,
					reinterpret_cast<const bson_t*>(query.bson()),
					nullptr,
					nullptr);
  return new QMongoCursor(cursor, this);
}

QMongoCursor *QMongoCollection :: find(QBson const &query, QBson const &fields)
{
  auto flags = MONGOC_QUERY_NONE;
  auto cursor = mongoc_collection_find (this->collection(),
					flags,
					0,
					0,
					0,
					reinterpret_cast<const bson_t*>(query.bson()),
					reinterpret_cast<const bson_t*>(fields.bson()),
					nullptr);
  return new QMongoCursor(cursor, this);
}
_mongoc_collection_t *QMongoCollection :: collection() const
{
  return d->collection.data();
}
  
bool QMongoCollection :: insert(QBson const &document)
{
  bson_error_t error;
  auto flags = MONGOC_INSERT_NONE;
  auto ret = mongoc_collection_insert (this->collection(),
				       flags,
				       reinterpret_cast<const bson_t *>(document.bson()),
				       d->write_concern.data(),
				       &error);
  handleError (error);
  return ret;
}


NAMESPACE_SOFT_END
