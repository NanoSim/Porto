#ifndef MONGOURIPROTOTYPE_H_DEF
#define MONGOURIPROTOTYPE_H_DEF

#include <QObject>
#include <QScriptable>
#include <QScriptValue>
#include <qmongouri.h>

using soft::mongo::Uri;
class MongoUriPrototype : public QObject
			, public QScriptable
{
  Q_OBJECT

public:
  explicit MongoUriPrototype (QObject *parent = nullptr);
  virtual ~MongoUriPrototype();

public slots:
  bool ssl() const;
  QScriptValue copy() const;
  QScriptValue database() const;
  QScriptValue string() const;
  QScriptValue username() const;
  QScriptValue password() const;
  QScriptValue hosts() const;

private:
  Uri *self() const;
};


#endif // MONGOURIPROTOTYPE_H_DEF
