#ifndef QMONGOCLIENT_H_DEF
#define QMONGOCLIENT_H_DEF

#include <QObject>
#include <QStringList>
#include <memory>
#include <mongoc.h>
#include "mongons.h"
#include "qbson.h"
#include "qmongouri.h"

SOFT_BEGIN_NAMESPACE
MONGO_BEGIN_NAMESPACE

class Collection;
class Database;
class GridFS;
class WriteConcern;
class ReadPrefs;
class Cursor;
class Uri;
class Client : public QObject
{
   Q_OBJECT

public:
   Client (QObject * parent = nullptr);
   Client (const QString & uri, QObject * parent = nullptr);
   Client (mongoc_client_t *client, QObject * parent = nullptr);
   Client (Client const & other);
   Client (Client && other);
   virtual ~Client();

   static void initialize();
   static void cleanup();
   WriteConcern *writeConcern ();
   void setWriteConcern (WriteConcern const * writeConcern);
   ReadPrefs *readPrefs();
   void setReadPrefs (ReadPrefs const * readPrefs);
   Collection *collection(const char *databaseName, const char *collectionName);
   Database *database(const char *databaseName);
   GridFS *gridFS(const char *databaseName, const char *prefix);
   Uri* uri();
   QStringList databaseNames() const;
   
   Cursor *command(const char* dbname,
                   soft::bson::Bson* query,
                   soft::bson::Bson* fields);
                                                    
   bool commandSimple(const char* dbname,
                      soft::bson::Bson* command,
                      soft::bson::Bson** reply);
 
   void destroy();

protected:
   friend class ClientPool;
   mongoc_client_t *data() const;

private:
   std::shared_ptr<mongoc_client_t> client;
};

MONGO_END_NAMESPACE
SOFT_END_NAMESPACE

#endif // QMONGOCLIENT_H_DEF
