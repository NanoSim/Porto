#include "bsonprototype.h"
#include "qbson.h"
#include <QtScript>
#include <QtCore>

using soft::bson::Bson;

BsonPrototype :: BsonPrototype(QObject *parent)
   : QObject(parent)
{}

BsonPrototype ::  ~BsonPrototype()
{}

Bson *BsonPrototype :: self() const
{
   Bson *bsonUri = qscriptvalue_cast<Bson*>(thisObject());
   if (!bsonUri) {
      context()->throwError (QString (tr ("Cannot instanciate Bson")));
   }
   return bsonUri;
}

QScriptValue BsonPrototype :: asString() const
{
   return engine()->toScriptValue (self()->asString());
}
