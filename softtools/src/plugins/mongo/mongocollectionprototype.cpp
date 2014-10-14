#include <QtCore>
#include <QtScript>

#include <qbson.h>
#include "qmongoreadprefs.h"
#include <qmongocollection.h>
#include "mongocollectionprototype.h"

using soft::bson::Bson;
using soft::mongo::Collection;
using soft::mongo::ReadPrefs;

namespace {
static Bson* bsonFromScriptValue(QScriptValue const &bson, MongoCollectionPrototype *parent)
{
  // If the scriptvalue type of the bson argument is a string, just pass it
  // to the new Bson instance.  
  if (bson.isString()) {
    auto json = bson.toString();
    auto bsonObj = new Bson(json, parent);
    return bsonObj;
  }

  // If the scriptvalue type of the bson argument is an object, but
  // not a QObject, it is a javascript object, and can be stringified
  // before it is passed to the new Bson instance.
  // WARNING: There is a performance penalty here!
 
  if (bson.isObject() && !bson.isQObject()) {
      auto context = parent->engine()->pushContext();
      context->activationObject().setProperty("__o", bson);
      auto json = parent->engine()->evaluate("JSON.stringify(__o)").toString();
      parent->engine()->popContext();
      auto bsonObj = new Bson(json, parent);
      return bsonObj;
  }  

  // If bson is a QObject, try to cast it to a Bson* instance and just
  // return the object.
  if (bson.isQObject()) {
    auto bsonObj = qscriptvalue_cast<Bson*>(bson);
    if(bsonObj) {
      return bsonObj;
    }
  }

  return nullptr;
}
}
MongoCollectionPrototype :: MongoCollectionPrototype(QObject * parent)
   : QObject(parent)
{}
 
MongoCollectionPrototype :: ~MongoCollectionPrototype()
{}

Collection *MongoCollectionPrototype :: self() const
{
  auto coll = qscriptvalue_cast<Collection*> (thisObject());
  if (!coll) {
    context()->throwError (QString (tr ("Cannot instanciate MongoCollection")));
  }
  return coll;
}

/*!
  \brief Execute an 'aggregation' query

  The aggregate() function shall execute an aggregation query on the
  underlying collection. The bson pipeline is not validated, simply
  passed along as appropriate to the server. As such, compatibility
  and errors should be validated in the appropriate server
  documentation.
 */
QScriptValue MongoCollectionPrototype :: aggregate(QMongoQueryFlags::QueryFlags const &flags, QScriptValue const &pipeline, QScriptValue const &readPrefs)
{
  Q_UNUSED(flags);
  Q_UNUSED(pipeline);
  Q_UNUSED(readPrefs);
  return QScriptValue();
}

/*!
  \brief Executes a command on collection

  The command() function shall execute a command on a collection. This
  is performed lazily after calling MongoCursor::next() on the
  resulting cursor structure.

*/
QScriptValue MongoCollectionPrototype :: command (QMongoQueryFlags::QueryFlags const &flags, quint32 skip, quint32 limit, quint32 batch_size, QScriptValue const &query, QScriptValue const &fields, QScriptValue const & readPrefs)
{
  Q_UNUSED(flags);
  Q_UNUSED(skip);
  Q_UNUSED(limit);
  Q_UNUSED(batch_size);
  Q_UNUSED(query);
  Q_UNUSED(fields);
  Q_UNUSED(readPrefs);
  return QScriptValue();
}

QScriptValue MongoCollectionPrototype :: insert(QScriptValue const &bson)
{
   auto b = bsonFromScriptValue(bson, this);
   return self()->insert(b);
}

QScriptValue MongoCollectionPrototype :: name() const
{
  return engine()->toScriptValue (self()->name());
}

bool MongoCollectionPrototype :: drop() 
{
  return self()->drop();
}

bool MongoCollectionPrototype :: createIndex(QScriptValue const &bson)
{
  auto bsonObj = bsonFromScriptValue(bson, this);  
  if (!bsonObj) {
    context()->throwError("Cannot instanciante Bson");
    return false;
  }
  
  return self()->createIndex(bsonObj);
}

bool MongoCollectionPrototype :: update(QScriptValue const &bson, QScriptValue const &upd)
{
  auto bson1 = bsonFromScriptValue(bson, this);  
  auto bson2 = bsonFromScriptValue(upd, this);  
  return self()->update(bson1, bson2);
}

bool MongoCollectionPrototype :: rename(QString const &newDatabaseName,
					QString const &newName)
{
  return self()->rename(qPrintable(newDatabaseName), 
			qPrintable(newName),
			false);
}

QScriptValue MongoCollectionPrototype :: find(QScriptValue const &query,
					      QScriptValue const &fields)
{
   auto q = bsonFromScriptValue(query, this);
   if (fields.isValid()) {
      auto f = bsonFromScriptValue(fields, this);
      return engine()->toScriptValue (self()->find(q, f));
   }
   return engine()->toScriptValue (self()->find(q));
}

QScriptValue MongoCollectionPrototype :: find()
{
  return engine()->toScriptValue (self()->find(nullptr, nullptr));;
}

qint64 MongoCollectionPrototype :: count(QScriptValue const &query)
{
  return self()->count (bsonFromScriptValue (query, this));
}
