#include <QtCore>
#include <QScriptEngine>
#include <QScriptContext>
#include <QScriptValue>
#include <functional>
#include <mongons.h>

#include <QMongo>
#include "mongoplugin.h"
#include "bsonprototype.h"
#include "mongoclientprototype.h"
#include "mongocollectionprototype.h"
#include "mongodatabaseprototype.h"
#include "mongouriprototype.h"
#include "mongoreadprefsprototype.h"
#include "mongocursorprototype.h"
#include "register.hh"

using soft::mongo::Client;
using soft::mongo::Collection;
using soft::mongo::Database;
using soft::mongo::WriteConcern;
using soft::mongo::ReadPrefs;
using soft::mongo::GridFS;
using soft::mongo::Uri;
using soft::mongo::Cursor;
using soft::bson::Bson;

Q_DECLARE_METATYPE (soft::mongo::Client*)
Q_DECLARE_METATYPE (soft::mongo::Collection*)
Q_DECLARE_METATYPE (soft::mongo::Database*)
Q_DECLARE_METATYPE (soft::mongo::WriteConcern*)
Q_DECLARE_METATYPE (soft::mongo::ReadPrefs*)
Q_DECLARE_METATYPE (soft::mongo::GridFS*)
Q_DECLARE_METATYPE (soft::mongo::Uri*)
Q_DECLARE_METATYPE (soft::mongo::Cursor*)
Q_DECLARE_METATYPE (soft::mongo::Bson*)

MongoPlugin :: ~MongoPlugin()
{
  soft::mongo::cleanup();
}

static QScriptValue createMongoCollection (QScriptContext *, QScriptEngine *engine)
{
  auto cls = new Collection();
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


void MongoPlugin :: registerPlugin (QScriptEngine *engine)
{
   soft::mongo::initialize();

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
