#ifndef MONGODATABASEPROTOTYPE_H_DEF
#define MONGODATABASEPROTOTYPE_H_DEF

#include <QObject>
#include <QScriptable>
#include <QScriptValue>
#include <qmongodatabase.h>

using porto::mongo::Database;
class MongoDatabasePrototype : public QObject
			     , public QScriptable
{
  Q_OBJECT

public:
  explicit MongoDatabasePrototype (QObject *parent = nullptr);
  virtual ~MongoDatabasePrototype();

public slots:
  bool addUser (QString const &username, QString const &password);
  bool removeUser (QString const &username);
  bool removeAllUsers();
  bool createCollection (QString const &name);
  bool hasCollection (QString const &name) const;
  bool drop();
  QScriptValue name() const;
  QScriptValue collection (QString const &name);
  QScriptValue collectionNames() const;
  

private:
  Database *self() const;
};

#endif // MONGODATABASEPROTOTYPE_H_DEF
