#include <QStringList>
#include "qmongodatabase.h"
#include "qmongocollection.h"
#include <mongoc.h>

PORTO_BEGIN_NAMESPACE
MONGO_BEGIN_NAMESPACE

static void databaseDeleter(mongoc_database_t *ptr)
{
  mongoc_database_destroy (ptr);
}

Database :: Database (QObject * parent)
   : QObject (parent)
{}

Database :: Database(mongoc_database_t * database, QObject * parent)
  : QObject (parent)
  , database (database, databaseDeleter)
{
}

Database :: Database (Database const &other)
  : QObject (other.parent())
  , database (other.database)
{
}

Database :: Database (Database && other)
  : QObject (std::move (other.parent()))
  , database (std::move (other.database))
{
}

bool Database :: addUser (char const * username, char const * password)
{
  bson_error_t error;
  auto ok = 
    mongoc_database_add_user (database.get(),
                              username,
                              password,
                              nullptr,
                              nullptr,
                              &error);
  return ok;
}

bool Database :: removeUser (char const * username)
{
  bson_error_t error;
  auto ok =
    mongoc_database_remove_user(database.get(),
                                username,
                                &error);
  return ok;
}

bool Database :: removeAllUsers ()
{
  bson_error_t error;
  auto ok =
    mongoc_database_remove_all_users (database.get(),
                                      &error);
  return ok;
}

bool Database :: drop()
{
  bson_error_t error;
  auto ok = mongoc_database_drop (database.get(),
                                  &error);
  return ok;
}


QString Database :: name() const {
   return QString(mongoc_database_get_name( database.get() ));
}

bool Database :: createCollection(const char * name) {
  bson_error_t error;
  auto ok = mongoc_database_create_collection (database.get(),
                                               name,
                                               0, /* options */
                                               &error);
  return ok;
}

bool Database :: hasCollection(QString const & name) const
{
  bson_error_t error;
  return mongoc_database_has_collection(database.get(),
					qPrintable(name),
					&error);
}

Collection *Database :: collection(QString const &name)
{
   auto coll = mongoc_database_get_collection (database.get(),
					       qPrintable(name));
   if (coll == nullptr)
      return nullptr;

   auto collection = new Collection(coll, this);
   return collection;
}

QStringList Database :: collectionNames() const
{
   bson_error_t error;
   char **names;
   QStringList ret;
   
   if ((names = mongoc_database_get_collection_names (database.get(),
						      &error))) {
      for (auto i = 0; names[i]; ++i) {
	 ret << names[i];
      }
   }

   bson_strfreev (names);
   return ret;
}

Database :: ~Database()
{}

MONGO_END_NAMESPACE
PORTO_END_NAMESPACE

