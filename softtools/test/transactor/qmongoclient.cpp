#include <mongoc.h>
#include <QDebug>

#include "qmongocollection.h"
#include "qmongoclient.h"

NAMESPACE_SOFT_BEGIN

struct ClientDeleter {
  static inline void cleanup (mongoc_client_t *ptr) {
    mongoc_client_destroy (ptr);
  }
};

class QMongoClient :: Private {
  friend class QMongoClient;

  Private (const QString &uri = QString("mongodb://localhost"))
    : client (mongoc_client_new (qPrintable(uri)))
  {}
  
  QScopedPointer <mongoc_client_t, ClientDeleter> client; 
};

QMongoClient :: QMongoClient (QObject * parent)
  : QObject (parent)
  , d (new QMongoClient::Private())
{}

QMongoClient :: QMongoClient (const QString &uri, QObject * parent)
  : QObject (parent)
  , d (new QMongoClient::Private(uri))
{}

QMongoCollection *QMongoClient :: collection (const QString &dbName, const QString &collName)
{
  auto coll = mongoc_client_get_collection (this->client(), qPrintable (dbName), qPrintable (collName));
  auto ret = new QMongoCollection (coll, this);
  return ret;
}

QMongoClient :: ~QMongoClient()
{
  delete d;
}

void QMongoClient :: init()
{
  mongoc_init ();
}

void QMongoClient :: cleanup()
{
  mongoc_cleanup ();
}

_mongoc_client_t *QMongoClient :: client () const
{
  return d->client.data();
}

NAMESPACE_SOFT_END
