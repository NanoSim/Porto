#ifndef BSONPROTOTYPE_H_DEF
#define BSONPROTOTYPE_H_DEF

#include <QScriptable>
#include <QObject>
#include <QScriptValue>
#include <qbson.h>

using soft::bson::Bson;
class BsonPrototype : public QObject,
		      public QScriptable
{
   Q_OBJECT
public:
   BsonPrototype(QObject *parent = nullptr);
   virtual ~BsonPrototype();

public slots:
   QScriptValue asString() const;

private:
   Bson *self() const;
}; // class BsonPrototype

#endif // BSONPROTOTYPE_H_DEF
