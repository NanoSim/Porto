#include <QtScript>
#include <QtCore>

#include "mongouriprototype.h"

using porto::mongo::Uri;

MongoUriPrototype :: MongoUriPrototype (QObject *parent)
  : QObject (parent)
{}

MongoUriPrototype :: ~MongoUriPrototype()
{}

Uri *MongoUriPrototype :: self() const
{
  Uri *mongoUri = qscriptvalue_cast<Uri*>(thisObject());
  if (!mongoUri) {
    context()->throwError (QString (tr ("Cannot instanciate MongoUri")));
  }
  return mongoUri;
}

bool MongoUriPrototype :: ssl() const
{
  return self()->ssl();
}

QScriptValue MongoUriPrototype :: copy() const
{
  return engine()->toScriptValue (self()->copy());
}

QScriptValue MongoUriPrototype :: database() const
{
  return engine()->toScriptValue (self()->database());
}

QScriptValue MongoUriPrototype :: string() const
{
  return engine()->toScriptValue (self()->string());
}

QScriptValue MongoUriPrototype :: username() const
{
  return engine()->toScriptValue (self()->username());
}

QScriptValue MongoUriPrototype :: password() const
{
  return engine()->toScriptValue (self()->password());
}

QScriptValue MongoUriPrototype :: hosts() const
{
  return qScriptValueFromSequence (engine(), self()->hosts());
}

