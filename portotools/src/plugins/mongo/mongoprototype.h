#ifndef MONGOPROTOTYPE_H_DEF
#define MONGOPROTOTYPE_H_DEF

#include <QScriptable>
#include <QObject>
#include <QScriptValue>

class MongoPrototype : public QObject, public QScriptable
{
   Q_OBJECT
public:
   MongoPrototype(QObject *parent = 0);
   virtual ~MongoPrototype();

public slots:
   QScriptValue connect(QString const &uri);
}; // class MongoPrototype

#endif // MONGOPROTOTYPE_H_DEF
