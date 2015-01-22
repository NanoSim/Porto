#ifndef QMONGOCLIENT_H_DEF
#define QMONGOCLIENT_H_DEF

#include <QObject>
#include "softns.h"

struct _mongoc_client_t;
NAMESPACE_SOFT_BEGIN

class QMongoCollection;
class QMongoClient : public QObject
{
  Q_OBJECT

public:
  explicit QMongoClient (QObject * parent = nullptr);
  QMongoClient (const QString &uri, QObject * parent = nullptr);
  virtual ~QMongoClient();
  static void init();
  static void cleanup();

public slots:
  QMongoCollection *collection (const QString &dbname, const QString &collname);
  
protected:
  _mongoc_client_t *client () const;
  
private:
  class Private;
  Private *d;  
};

NAMESPACE_SOFT_END

Q_DECLARE_METATYPE (NAMESPACE_SOFT_NAME::QMongoClient*)

#endif // QMONGOCLIENT_H_DEF
