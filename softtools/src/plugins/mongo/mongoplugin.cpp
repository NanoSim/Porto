#include <QtCore>
#include <QScriptEngine>
#include <QScriptContext>
#include <QScriptValue>
#include <functional>
#include <mongons.h>

#include "mongoplugin.h"
#include "qbson.h" 
#include "qmongoinitialize.h"
#include "qmongoclient.h"
#include "qmongocollection.h"
#include "qmongodatabase.h"
#include "qmongowriteconcern.h"
#include "qmongoreadprefs.h"
#include "qmongogridfs.h"
#include "qmongouri.h"
#include "qmongocursor.h"
#include "bsonprototype.h"
#include "mongoclientprototype.h"
#include "mongocollectionprototype.h"
#include "mongodatabaseprototype.h"
#include "mongouriprototype.h"
#include "mongoreadprefsprototype.h"
#include "mongocursorprototype.h"

using porto::mongo::Client;
using porto::mongo::Collection;
using porto::mongo::Database;
using porto::mongo::WriteConcern;
using porto::mongo::ReadPrefs;
using porto::mongo::GridFS;
using porto::mongo::Uri;
using porto::mongo::Cursor;
using porto::bson::Bson;

Q_DECLARE_METATYPE (porto::mongo::Client*)
Q_DECLARE_METATYPE (porto::mongo::Collection*)
Q_DECLARE_METATYPE (porto::mongo::Database*)
Q_DECLARE_METATYPE (porto::mongo::WriteConcern*)
Q_DECLARE_METATYPE (porto::mongo::ReadPrefs*)
Q_DECLARE_METATYPE (porto::mongo::GridFS*)
Q_DECLARE_METATYPE (porto::mongo::Uri*)
Q_DECLARE_METATYPE (porto::mongo::Cursor*)
Q_DECLARE_METATYPE (porto::bson::Bson*)

MongoPlugin :: ~MongoPlugin()
{}

static QScriptValue createMongoCollection (QScriptContext *, QScriptEngine *engine)
{
  auto cls = new Collection();
  return engine->newQObject(cls, QScriptEngine::ScriptOwnership);
}

template <class T>
static QScriptValue creator (QScriptContext *, QScriptEngine *engine)
{
  auto cls = new T();
  return engine->newQObject(cls, QScriptEngine::ScriptOwnership);
}

static QScriptValue createBson (QScriptContext *ctx, QScriptEngine *engine)
{
  QString arg("{}");
  if (ctx->argumentCount() > 0) {
    arg = ctx->argument(0).toString();
  }
  auto cls = new Bson(arg);
  return engine->newQObject(cls, QScriptEngine::ScriptOwnership);
}

static QScriptValue createMongoClient (QScriptContext *ctx, QScriptEngine *engine)
{
  QString arg("mongodb://localhost");
  if (ctx->argumentCount() > 0) {
    arg = ctx->argument(0).toString();
  }
  auto c = new Client(arg);
  return engine->newQObject(c, QScriptEngine::ScriptOwnership);
}

template <class T>
QScriptValue objectContructor(QScriptContext *ctx,
			      QScriptEngine *e)
{   
   auto parent = ctx->argument(0).toQObject(); 
   auto object = q_check_ptr (new T(parent));
   return e->newQObject(object, QScriptEngine::ScriptOwnership);   
}

template <class P, class T>
static void registerPrototype (QScriptEngine *engine)
{
   auto prototype = q_check_ptr (new P (engine));
   engine->setDefaultPrototype (qMetaTypeId <T*> (),
				engine->newQObject (prototype));
}

template <class T, class Fn>
static void registerConstructor (QScriptEngine *engine,
				 QString const & name,
				 Fn construct)
{
   auto ctor    = engine->newFunction(construct);
   auto metaObj = engine->newQMetaObject(&T::staticMetaObject, ctor);
   engine->globalObject().setProperty(name, metaObj);
}

void MongoPlugin :: registerPlugin (QScriptEngine *engine)
{
   porto::mongo::initialize();

   registerPrototype<MongoClientPrototype,     Client>     (engine);
   registerPrototype<MongoCollectionPrototype, Collection> (engine);
   registerPrototype<MongoDatabasePrototype,   Database>   (engine);
   registerPrototype<MongoUriPrototype,        Uri>        (engine);
   registerPrototype<MongoReadPrefsPrototype,  ReadPrefs>  (engine);
   registerPrototype<MongoCursorPrototype,     Cursor>     (engine);
   registerPrototype<BsonPrototype,            Bson>       (engine);
   registerConstructor<Client>     (engine, "MongoClient", createMongoClient);
   registerConstructor<Collection> (engine, "MongoCollection", createMongoCollection);
   registerConstructor<Database>   (engine, "MongoDatabase", creator<Database>);
   registerConstructor<Bson>       (engine, "Bson", createBson);
   registerConstructor<Uri>        (engine, "MongoUri", creator<Uri>);
   registerConstructor<ReadPrefs>  (engine, "MongoReadPrefs", creator<ReadPrefs>);
   registerConstructor<Cursor>     (engine, "MongoCursor", creator<Cursor>);
}
