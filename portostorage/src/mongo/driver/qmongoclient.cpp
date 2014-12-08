#include <QtDebug>
#include "qmongoclient.h"
#include "qmongocollection.h"
#include "qmongodatabase.h"
#include "qmongogridfs.h"
#include "qmongowriteconcern.h"
#include "qmongoreadprefs.h"
#include "qmongocursor.h"
#include <iostream>

PORTO_BEGIN_NAMESPACE
MONGO_BEGIN_NAMESPACE

static void clientDeleter(mongoc_client_t *ptr)
{
   mongoc_client_destroy(ptr);
}

Client :: Client (QObject * parent)
   : QObject (parent)
{}

Client :: Client (const QString &uri, QObject * parent)
   : QObject (parent)
   , client(mongoc_client_new(qPrintable(uri)), clientDeleter)
{}

Client :: Client (mongoc_client_t *client, QObject * parent)
   : QObject (parent)
   , client(client, clientDeleter)
{}


Client::Client(const Client & other)
   : QObject (other.parent())
   , client(other.client)
{}

Client::Client(Client &&o)
   : QObject (std::move(o.parent()))
   , client(std::move(o.client))
{}


Client::~Client()
{}

Collection * Client :: collection(const char *db, const char *collname)
{
   auto c = new Collection(mongoc_client_get_collection(client.get(), db, collname),
                           this);
   return c;
}

Database * Client :: database (const char *databaseName)
{
  auto d = new Database (mongoc_client_get_database (client.get(), databaseName),
                         this);
  return d;
}

GridFS * Client :: gridFS(const char *databaseName, const char *prefix)
{
  // TODO: Implement fail check
   bson_error_t error;
   auto gridfs = mongoc_client_get_gridfs (client.get(), databaseName, prefix, &error);
   if (gridfs) {
     auto g = new GridFS (gridfs, this);
     return g;
   }

   return nullptr;
}

Uri* Client :: uri() 
{
   auto u = new Uri (mongoc_uri_copy (mongoc_client_get_uri (client.get())),
                     this);
   return u;  
}

QStringList Client :: databaseNames() const
{
   bson_error_t error;
   char ** names, ** ptr;
   QStringList ret;

   names = mongoc_client_get_database_names (client.get(), &error);

   if (!names) {
      return ret;
   }

   for (ptr = names; *ptr; ptr++) {
      ret << QString::fromLocal8Bit (*ptr);
   }

   bson_strfreev (names);
   return ret;
}

Cursor* Client :: command(const char* dbname,
                         porto::bson::Bson* query,
                         porto::bson::Bson* fields)
{
   mongoc_read_prefs_t *read_prefs
       = mongoc_read_prefs_new(MONGOC_READ_PRIMARY);   

   mongoc_cursor_t *cur = 
        mongoc_client_command (client.get(),
                               dbname,
                               MONGOC_QUERY_NONE,
                               0,
                               0,
                               0,
                               query->data(),
                               fields ? fields->data() : nullptr,
                               read_prefs);
    mongoc_read_prefs_destroy(read_prefs);
    return new Cursor(cur, this);
}
                                                      
bool Client :: commandSimple(const char* dbname,
                   porto::bson::Bson* command,
                   porto::bson::Bson** reply)
{
   bson_error_t error;
   bson_t* replyOut = bson_new();
   mongoc_read_prefs_t *read_prefs
       = mongoc_read_prefs_new(MONGOC_READ_PRIMARY);

   auto ret = mongoc_client_command_simple (client.get(),
                              dbname,
                              command->data(),
                              read_prefs,
                              replyOut,
                              &error);
   mongoc_read_prefs_destroy(read_prefs);
   *reply = nullptr;
   if(ret)
     *reply = new porto::bson::Bson(replyOut, this);
   else
      bson_destroy(replyOut);
   return ret; 
}


void Client :: destroy()
{
   mongoc_client_destroy (client.get());
}

mongoc_client_t *Client :: data() const
{
   return client.get();
}

WriteConcern * Client :: writeConcern ()
{
   auto writeconcernt = mongoc_client_get_write_concern (client.get());
   auto ret = new WriteConcern (writeconcernt, this);
   return ret;
}

void Client :: setWriteConcern (WriteConcern const * writeConcern)
{
   mongoc_client_set_write_concern 
      (client.get(), writeConcern->data());
}

ReadPrefs * Client :: readPrefs()
{
   auto readprefst = mongoc_client_get_read_prefs (client.get());
   auto ret = new ReadPrefs (readprefst, this);
   return ret;
}

void Client :: setReadPrefs (ReadPrefs const * readPrefs)
{
   mongoc_client_set_read_prefs 
      (client.get(), readPrefs->data());
}


void my_handler (mongoc_log_level_t  log_level,
		 const char         *log_domain,
		 const char         *message,
		 void               * /*user_data*/)
{
  //  qDebug() << "LOG: " << log_level << log_domain << message;
}

void Client :: initialize()
{
  mongoc_log_set_handler(my_handler, nullptr);
  mongoc_init();
}

void Client :: cleanup()
{
  mongoc_cleanup();
}

MONGO_END_NAMESPACE
PORTO_END_NAMESPACE
