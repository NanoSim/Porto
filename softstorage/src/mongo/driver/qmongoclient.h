#ifndef QMONGOCLIENT_H_DEF
#define QMONGOCLIENT_H_DEF

#include <QObject>
#include <QStringList>
#include <memory>
#include <mongoc.h>
#include "mongons.h"
#include "qbson.h"
#include "qmongouri.h"

PORTO_BEGIN_NAMESPACE
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
                   porto::bson::Bson* query,
                   porto::bson::Bson* fields);
                                                    
   bool commandSimple(const char* dbname,
                      porto::bson::Bson* command,
                      porto::bson::Bson** reply);
 
   void destroy();

protected:
   friend class ClientPool;
   mongoc_client_t *data() const;

private:
   std::shared_ptr<mongoc_client_t> client;
};

MONGO_END_NAMESPACE
PORTO_END_NAMESPACE

#endif // QMONGOCLIENT_H_DEF
