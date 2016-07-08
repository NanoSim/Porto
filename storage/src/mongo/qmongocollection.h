#pragma once

#include <QObject>
#include <memory>
#include <mongoc.h>
#include "mongons.h"
#include "qbson.h"
#include "qmongocursor.h"
#include "qmongowriteconcern.h"
#include "qmongoreadprefs.h"
#include <QVector>

SOFT_BEGIN_NAMESPACE
MONGO_BEGIN_NAMESPACE

using soft::bson::Bson;

class Collection : public QObject
{
   Q_OBJECT
   
public:   
   explicit Collection (QObject * parent = nullptr);
   Collection (mongoc_collection_t * coll, QObject * parent = nullptr);
   Collection (Collection const & other);
   Collection (Collection && other);
   ~Collection();

   QString name() const;
   bool drop ();
   bool createIndex(Bson* bson);
#if 0
   bool ensureIndex(Bson* bson);
#endif
   bool insert(Bson* bson);
   bool insertBulk(QVector<Bson*> bson);
   bool update(Bson* selector, Bson* upd);
   bool rename (QString const &newDatabaseName, 
		QString const &newName, 
		bool dropTargetBeforeRename);
   bool findAndModify(Bson* query,
                      Bson* sot,
                      Bson* update,
                      Bson* fields,
                      bool remove,
                      bool upsert,
                      Bson** reply);
   Cursor *find(Bson* query = nullptr,
                      Bson* fields = nullptr,
                      quint32 limit = 0);
   Cursor *aggregate(Bson* pipeline);
   Cursor *command(Bson* query, Bson* fields);
   bool command_simple(Bson* command);
   qint64 count(Bson* query) const;
   bool remove(Bson *query);
   WriteConcern writeConcern();
   void setWriteConcern(mongo::WriteConcern* wconc);
   ReadPrefs readPrefs();
   void setReadPrefs(mongo::ReadPrefs* rprefs);
   
   Bson validate(Bson* options, bool& retcode);
   bool save(Bson* document);
   
private:
   static bool Undefined;  // for default reference parameter initialization 
   std::shared_ptr<mongoc_collection_t> collection;
};

MONGO_END_NAMESPACE
SOFT_END_NAMESPACE

