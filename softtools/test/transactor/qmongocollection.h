#ifndef QMONGOCOLLECTION_H_DEF
#define QMONGOCOLLECTION_H_DEF

#include <QObject>
#include "softns.h"

struct _mongoc_collection_t;
NAMESPACE_SOFT_BEGIN

class QBson;
class QMongoCursor;

class QMongoCollection : public QObject
{
  Q_OBJECT
  
public:
  explicit QMongoCollection (QObject * parent = nullptr);
  QMongoCollection (_mongoc_collection_t *collection, QObject * parent = nullptr);
  virtual ~QMongoCollection();

protected:
  _mongoc_collection_t *collection() const;
					  
public slots:
  QMongoCursor *find(QBson const &query);
  QMongoCursor *find(QBson const &query, QBson const &fields);
  bool insert(QBson const &document);
  
private:
  class Private;
  Private *d;
};

NAMESPACE_SOFT_END

Q_DECLARE_METATYPE (NAMESPACE_SOFT_NAME::QMongoCollection*)

#endif // QMONGOCOLLECTION_H_DEF
