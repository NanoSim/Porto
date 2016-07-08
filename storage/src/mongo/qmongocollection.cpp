#include "qmongocollection.h"
#include <mongoc.h>
#include <iostream>

SOFT_BEGIN_NAMESPACE
MONGO_BEGIN_NAMESPACE

#define NOT_IMPLEMENTED throw std::runtime_error("Not implemented");

bool Collection :: Undefined = false;

using soft::bson::Bson;

static void collectionDeleter(mongoc_collection_t *ptr)
{
   mongoc_collection_destroy(ptr);
}

Collection::Collection(QObject * parent)
   : QObject(parent)
{}

Collection::Collection (mongoc_collection_t * coll, QObject * parent)
   : QObject(parent)
   , collection(coll, collectionDeleter)
{}

Collection::Collection (Collection const & other)
   : QObject(other.parent())
   , collection(other.collection)
{}

Collection::Collection (Collection && other)
   : QObject (std::move(other.parent()))
   , collection(std::move(other.collection))
{}

Collection::~Collection()
{}

bool Collection :: drop ()
{
    bson_error_t error;
    auto isOk = mongoc_collection_drop (collection.get(), &error);
    return isOk;
}

bool Collection :: createIndex(Bson* bson)
{
    bson_error_t error;
    const mongoc_index_opt_t* iopt = mongoc_index_opt_get_default();
    auto isOk = mongoc_collection_create_index (collection.get(),
                                                bson->data(),
                                                iopt,
                                                &error);
    return isOk;
}

#if 0
bool Collection :: ensureIndex(Bson* bson)
{
    bson_error_t error;
    const mongoc_index_opt_t* iopt = mongoc_index_opt_get_default();
    auto isOk = mongoc_collection_ensure_index (collection.get(),
                                                bson->data(),
                                                iopt,
                                                &error);
    return isOk;
}
#endif

bool Collection :: insert(Bson* bson)
{
    bson_error_t error;
    mongoc_insert_flags_t flags = MONGOC_INSERT_NONE;
    mongoc_write_concern_t* wconc = mongoc_write_concern_new();
    auto isOk = mongoc_collection_insert (collection.get(),
                                                flags,
                                                bson->data(),
                                                wconc,
                                                &error);
    mongoc_write_concern_destroy(wconc);
    return isOk;
}

bool Collection :: insertBulk(QVector<Bson*> bson)
{
  NOT_IMPLEMENTED
#if 0
    bson_error_t error;
    mongoc_insert_flags_t flags = MONGOC_INSERT_NONE;
    quint32 n = bson.size();
    typedef const bson_t* const_bson_ptr;
    auto bulk = new const_bson_ptr[n];

    if(bulk == nullptr)
       return false;
    for(quint32 i = 0; i < n; ++i)
       bulk[i] = bson[i]->data();

    mongoc_write_concern_t* wconc = mongoc_write_concern_new();

    auto isOk = mongoc_collection_insert_bulk (collection.get(),
                                              flags,
                                              bulk,
                                              n,
                                              wconc,
                                              &error);
    mongoc_write_concern_destroy(wconc);
    delete [] bulk;
    return isOk;
#endif
    return false;
}

bool Collection :: remove(Bson * selector)
{
    bson_error_t error;
    mongoc_remove_flags_t flags = MONGOC_REMOVE_SINGLE_REMOVE;
    mongoc_write_concern_t* wconc = NULL;
    auto isOk = mongoc_collection_remove(collection.get(),
					 flags,
					 selector->data(),
					 wconc,
					 &error);
    return isOk;
}

bool Collection :: update(Bson* selector, Bson* upd)
{
    bson_error_t error;
    mongoc_update_flags_t flags = MONGOC_UPDATE_NONE;
    mongoc_write_concern_t* wconc = mongoc_write_concern_new();
    
    auto isOk = mongoc_collection_update (collection.get(),
                          flags,
                          selector->data(),
                          upd->data(),
                          wconc,
                          &error);
    mongoc_write_concern_destroy(wconc);
    return isOk;
}

bool Collection :: rename (QString const &newDatabaseName, 
			   QString const &newName, 
			   bool dropTargetBeforeRename)
{
   bson_error_t error;
   auto isOk = mongoc_collection_rename (collection.get(),
					 qPrintable(newDatabaseName),
					 qPrintable(newName),
					 dropTargetBeforeRename,
					 &error);
   return isOk;
}

bool Collection :: findAndModify(Bson* query,
                      Bson* sort,
                      Bson* update,
                      Bson* fields,
                      bool remove,
                      bool upsert,
                      Bson** reply)
{
   bson_t* replyOut = bson_new();
   bson_error_t error;
   
   bool return_new = false; //avoid garbage, don't expose this parameter
   
   auto ret = mongoc_collection_find_and_modify (collection.get(),
                                                 query->data(),
                                                 sort->data(),
                                                 update->data(),
                                                 fields->data(),
                                                 remove,
                                                 upsert,
                                                 return_new,
                                                 replyOut,
                                                 &error);
    *reply = nullptr;
    if(ret)
       *reply = new Bson(replyOut);
    else
       bson_destroy(replyOut);
    return ret;
}

Cursor *Collection :: find(Bson* query, Bson* fields, quint32 limit)
{
   auto cursor = new Cursor(mongoc_collection_find(collection.get(),
						   MONGOC_QUERY_NONE,
						   0,
						   0,
						   0,
						   query->data(),
						   nullptr,
						   nullptr),
			    this);
   return cursor;
}


QString Collection :: name() const
{
   return QString(mongoc_collection_get_name(collection.get()));
}


Cursor *Collection :: aggregate(Bson* pipeline)
{
    mongoc_cursor_t* cur = mongoc_collection_aggregate (collection.get(),
                                                MONGOC_QUERY_NONE,
                                                pipeline->data(),
                                                nullptr,
                                                nullptr);
    return new Cursor(cur, this);
}

Cursor *Collection :: command(Bson* query, Bson* fields)
{
    mongoc_cursor_t* cur = mongoc_collection_command(collection.get(),
                                                MONGOC_QUERY_NONE,
                                                0,
                                                0,
                                                0,
                                                query->data(),
                                                fields->data(),
                                                nullptr);
    return new Cursor(cur, this);
}

bool Collection :: command_simple(Bson* command)
{
   bson_error_t error;
   return mongoc_collection_command_simple(collection.get(),
                                           command->data(),
                                           nullptr,
                                           nullptr,
                                           &error);
}

qint64 Collection :: count(Bson* query) const
{
    bson_error_t error;
    return mongoc_collection_count (collection.get(),
                         MONGOC_QUERY_NONE,
                         query->data(),
                         0,
                         0,
                         nullptr,
                         &error);
}

WriteConcern Collection :: writeConcern()
{
   return WriteConcern( 
      const_cast<mongoc_write_concern_t*>
         (mongoc_collection_get_write_concern(collection.get())));
}

void Collection :: setWriteConcern (WriteConcern* wconc)
{
   mongoc_collection_set_write_concern(collection.get(), wconc->data());
}

ReadPrefs Collection :: readPrefs()
{
   return ReadPrefs(
      const_cast<mongoc_read_prefs_t*>( mongoc_collection_get_read_prefs(collection.get())));
}

void Collection :: setReadPrefs(ReadPrefs* rprefs)
{
   mongoc_collection_set_read_prefs (collection.get(), rprefs->data());
}

Bson Collection :: validate(Bson* options, bool& retcode)
{
   bson_error_t error;
   bson_t* reply = bson_new();
   retcode = mongoc_collection_validate( collection.get(),
                                         options->data(),
                                         reply,
                                         &error);
   return Bson(reply);
}

bool Collection :: save(Bson* document)
{
   bson_error_t error;
   mongoc_write_concern_t* wconc = mongoc_write_concern_new();
   bool retcode = mongoc_collection_save( collection.get(),
                                         document->data(),
                                         wconc,
                                         &error);
   return retcode;
}

MONGO_END_NAMESPACE
SOFT_END_NAMESPACE
