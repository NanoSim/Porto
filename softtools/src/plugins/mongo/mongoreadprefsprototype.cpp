#include <QtScript>
#include <QtCore>

#include "mongoreadprefsprototype.h"

using porto::mongo::ReadPrefs;

MongoReadPrefsPrototype :: MongoReadPrefsPrototype (QObject *parent)
  : QObject (parent)
{}

MongoReadPrefsPrototype :: ~MongoReadPrefsPrototype()
{}

ReadPrefs *MongoReadPrefsPrototype :: self() const
{
  ReadPrefs *mongoReadPrefs = qscriptvalue_cast<ReadPrefs*>(thisObject());
  if (!mongoReadPrefs) {
    context()->throwError (QString (tr ("Cannot instanciate MongoReadPrefs")));
  }
  return mongoReadPrefs;
}
