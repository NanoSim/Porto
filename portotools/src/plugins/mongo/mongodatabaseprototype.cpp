#include <QtScript>
#include <QtCore>

#include "qmongocollection.h"
#include "qmongodatabase.h"
#include "mongodatabaseprototype.h"

using porto::mongo::Database;

MongoDatabasePrototype :: MongoDatabasePrototype (QObject *parent)
  : QObject (parent)
{}

MongoDatabasePrototype :: ~MongoDatabasePrototype()
{}

Database *MongoDatabasePrototype :: self() const
{
  Database *mongoDatabase = qscriptvalue_cast<Database*>(thisObject());
  if (!mongoDatabase) {
    context()->throwError (QString (tr ("Cannot instanciate MongoDatabase")));
  }
  return mongoDatabase;
}

bool MongoDatabasePrototype :: addUser (QString const &username, QString const &password)
{
  return self()->addUser(qPrintable(username),
			 qPrintable(password));
}

bool MongoDatabasePrototype :: removeUser (QString const &username)
{
  return self()->removeUser(qPrintable(username));
}

bool MongoDatabasePrototype :: removeAllUsers()
{
  return self()->removeAllUsers();
}

bool MongoDatabasePrototype :: createCollection (QString const &name)
{
  return self()->createCollection(qPrintable(name));
}

bool MongoDatabasePrototype :: hasCollection (QString const &name) const
{
  return self()->hasCollection(qPrintable(name));
}

bool MongoDatabasePrototype :: drop()
{
  return self()->drop();
}

QScriptValue MongoDatabasePrototype :: name() const
{
  return engine()->toScriptValue (self()->name());
}

QScriptValue MongoDatabasePrototype :: collection (QString const &name)
{
  return engine()->toScriptValue (self()->collection(qPrintable(name)));
}

QScriptValue MongoDatabasePrototype :: collectionNames() const
{
  return qScriptValueFromSequence(engine(), self()->collectionNames());
}

