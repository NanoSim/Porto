#include "mongoprototype.h"
#include "qmongo.h"
#include "qmongoclient.h"
#include <QtScript>
#include <QtCore>

MongoPrototype :: MongoPrototype(QObject *parent)
   : QObject(parent)
{
}

MongoPrototype :: ~MongoPrototype()
{
}

QScriptValue MongoPrototype :: connect(QString const &uri)
{
   QMongo *mongo = qscriptvalue_cast<QMongo*>(thisObject());
   if(mongo) {
      QMongoClient *client = mongo->connect(uri);
      QScriptEngine *engine = qobject_cast<QScriptEngine*>(this->parent());
      if(engine) {
	 return engine->toScriptValue(client);
      } 
   }

   return QScriptValue();
}
