#include "mongoclientprototype.h"
#include "qmongoclient.h"
#include "qmongocollection.h"
#include "qmongowriteconcern.h"
#include "qmongoreadprefs.h"
#include "qmongodatabase.h"
#include "qmongogridfs.h"
#include "qmongouri.h"
#include <QtScript>
#include <QtCore>

using soft::mongo::Client;
using soft::mongo::WriteConcern;
using soft::mongo::ReadPrefs;

MongoClientPrototype :: MongoClientPrototype(QObject *parent)
   : QObject (parent)
{}

MongoClientPrototype :: ~MongoClientPrototype()
{}

Client* MongoClientPrototype :: thisMongoClient() const
{
  Client* mongoClient = qscriptvalue_cast<Client*>(thisObject());
  if (!mongoClient) {
    context()->throwError (QString (tr ("Cannot instanciate MongoClient")));
  }
  
  return mongoClient;
}

QScriptValue MongoClientPrototype :: databaseNames() const
{
  return qScriptValueFromSequence(engine(), thisMongoClient()->databaseNames());
}

QScriptValue MongoClientPrototype :: collection (QString const &databaseName, QString const &collectionName)
{
  auto coll = thisMongoClient()->collection(qPrintable(databaseName),
					    qPrintable(collectionName));
  return engine()->toScriptValue(coll);
}

QScriptValue MongoClientPrototype :: writeConcern()
{
  return engine()->toScriptValue (thisMongoClient()->writeConcern());
}

QScriptValue MongoClientPrototype :: setWriteConcern(QScriptValue const &writeConcern)
{
  auto ptr = qscriptvalue_cast<WriteConcern*>(writeConcern);
  if (!ptr) {
    context()->throwError (QString (tr ("WriteConcern argument failure")));
  }
  thisMongoClient()->setWriteConcern (ptr);
  return engine()->undefinedValue();
}

QScriptValue MongoClientPrototype :: readPrefs()
{
  return engine()->toScriptValue (thisMongoClient()->readPrefs());
}

QScriptValue MongoClientPrototype :: setReadPrefs(QScriptValue const &readPrefs)
{
  auto ptr = qscriptvalue_cast<ReadPrefs*>(readPrefs);
  if (!ptr) {
    context()->throwError (QString (tr ("ReadPrefs argument failure")));
  }
  thisMongoClient()->setReadPrefs (ptr);
  return engine()->undefinedValue();
}

QScriptValue MongoClientPrototype :: database(QString const &databaseName)
{  
  return engine()->toScriptValue (thisMongoClient()->database(qPrintable(databaseName)));
}

QScriptValue MongoClientPrototype :: gridFS(QString const &databaseName, QString const &prefix)
{
  return engine()->toScriptValue (thisMongoClient()->gridFS(qPrintable(databaseName),
							    qPrintable(prefix)));
}

QScriptValue MongoClientPrototype :: uri()
{
  return engine()->toScriptValue (thisMongoClient()->uri());
}

