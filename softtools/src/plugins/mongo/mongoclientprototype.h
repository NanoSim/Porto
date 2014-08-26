#ifndef MONGOCLIENTPROTOTYPE_H_DEF
#define MONGOCLIENTPROTOTYPE_H_DEF

#include <QScriptable>
#include <QObject>
#include <QScriptValue>
#include <qmongoclient.h>

using soft::mongo::Client;
class MongoClientPrototype : public QObject, 
			     public QScriptable
{
  Q_OBJECT

public:
  explicit MongoClientPrototype(QObject *parent = nullptr);
  virtual ~MongoClientPrototype();
				 
public slots:
  QScriptValue databaseNames() const;
  QScriptValue collection(QString const &databaseName, QString const &collectionName);
  QScriptValue writeConcern();
  QScriptValue setWriteConcern(QScriptValue const &writeConcern);
  QScriptValue readPrefs();
  QScriptValue setReadPrefs(QScriptValue const &readPrefs);
  QScriptValue database(QString const &databaseName);
  QScriptValue gridFS(QString const &databaseName, QString const &prefix);
  QScriptValue uri();

private:  
  Client * thisMongoClient() const; 
};

#endif // MONGOCLIENTPROTOTYPE_H_DEF
