#pragma once

#include <QObject>
#include <memory>
#include <mongoc.h>
#include "mongons.h"

SOFT_BEGIN_NAMESPACE
MONGO_BEGIN_NAMESPACE

class Collection;
class Database : public QObject
{
   Q_OBJECT

public:
   explicit Database (QObject * parent = nullptr);
   Database (mongoc_database_t * database, QObject * parent = nullptr);
   Database (Database const &other);
   Database (Database && other);
   virtual ~Database();

   bool addUser (char const * username, char const * password);
   bool removeUser (char const * username);
   bool removeAllUsers ();
   bool createCollection(const char * name);
   bool hasCollection (QString const & name) const;

   Collection *collection(QString const &name);
   QStringList collectionNames() const;

   
   bool drop ();
   QString name() const;
private:
   std::shared_ptr<mongoc_database_t> database;
};

MONGO_END_NAMESPACE
SOFT_END_NAMESPACE
