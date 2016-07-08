#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <mongoc.h>

#include "bsonmodel.h"
#include "mongostrategy.h"

SOFT_BEGIN_NAMESPACE

const char *MongoStrategy::staticMetaType = "http://sintef.no/soft/TR/storage-strategy#mongo:0.1-SNAPSHOT-1";

const char *MongoStrategy::metaType() const
{
  return MongoStrategy::staticMetaType;
}

class MongoStrategy::Private
{
   friend class MongoStrategy;
   Private (QString const &uri, QString const &options)
      : uri (uri)
      , options (options)
      , client(mongoc_client_new (qPrintable(uri)))
   { 
      
      auto opt = options.split(";");
      foreach (auto o, opt) {
	 auto pair = o.split("=");
	 Q_ASSERT(pair.count() == 2);
	 optionMap.insert(pair[0], pair[1]);
      }

      QString db = (optionMap.contains("db") ? optionMap["db"] : "default");
      QString coll = (optionMap.contains("coll") ? optionMap["coll"] : "default");
      collection = mongoc_client_get_collection (client, 
						 qPrintable(db),
						 qPrintable(coll));
      Q_CHECK_PTR(collection);

   }
   ~Private()
   {
      mongoc_client_destroy (client);
      mongoc_collection_destroy (collection);
   }

   QString uri;
   QString options;
   mongoc_client_t *client;
   mongoc_collection_t *collection;
   QMap<QString, QString> optionMap;
};

MongoStrategy :: MongoStrategy()
   : IStorageStrategy ()
   , d(new MongoStrategy::Private(QString(),QString()))
{}

MongoStrategy :: MongoStrategy(const char *uri, const char *options)
   : IStorageStrategy(uri, options)
   , d (new MongoStrategy::Private(uri, options))
{
  // qDebug() << "MongoStrategy :: MongoStrategy" << uri << options;
}

MongoStrategy :: ~MongoStrategy()
{
   delete d;
}

IDataModel* MongoStrategy :: dataModel() const
{
  return (IDataModel*)new BsonModel();
}

void MongoStrategy :: store (IDataModel const *model)
{
   auto bsonModel = dynamic_cast<BsonModel const*>(model);

   bson_error_t error;
   auto retval = mongoc_collection_insert(d->collection, MONGOC_INSERT_NONE, bsonModel->bson(), 0, &error);
   Q_UNUSED(retval);
}

void MongoStrategy :: startRetrieve (IDataModel *model) const
{
   auto bsonModel = dynamic_cast<BsonModel const*>(model);
   bson_error_t error;
}

void MongoStrategy :: endRetrieve (IDataModel *model) const
{
}

IStorageStrategy* MongoStrategy :: create(char const *uri, char const *opts)
{
   auto s = new MongoStrategy(uri, opts);
   return s;
}


SOFT_END_NAMESPACE
