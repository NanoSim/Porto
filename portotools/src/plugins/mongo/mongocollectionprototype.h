#ifndef MONGOCOLLECTIONPROTOTYPE_H_DEF
#define MONGOCOLLECTIONPROTOTYPE_H_DEF

#include <QScriptable>
#include <QObject>
#include <QScriptValue>
#include <qmongoqueryflags.h>
#include <qmongocollection.h>

using porto::mongo::Collection;
class MongoCollectionPrototype : public QObject,
				 public QScriptable
{
  Q_OBJECT
public:
  explicit MongoCollectionPrototype (QObject *parent = nullptr);
  virtual ~MongoCollectionPrototype();

public slots:
  QScriptValue aggregate (QMongoQueryFlags::QueryFlags const &flags, QScriptValue const &pipeline, QScriptValue const &readPrefs);
  QScriptValue command (QMongoQueryFlags::QueryFlags const &flags, quint32 skip, quint32 limit, quint32 batch_size, QScriptValue const &query, QScriptValue const &fields, QScriptValue const & readPrefs);
  QScriptValue insert(QScriptValue const &bson);
  QScriptValue name() const;
  bool drop();
  bool createIndex(QScriptValue const &bson);
  bool update(QScriptValue const &bson, QScriptValue const &upd);
  bool rename(QString const &newDatabaseName,
	      QString const &newName);
  qint64 count(QScriptValue const &query);
  QScriptValue find(QScriptValue const &query,
		    QScriptValue const &fields = QScriptValue());
  QScriptValue find();  
		    
   

private:
  Collection *self() const;
}; //class MongoCollectionPrototype

#endif // MONGOCOLLECTIONPROTOTYPE_H_DEF
