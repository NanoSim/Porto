#ifndef MONGOREADPREFSPROTOTYPE_H_DEF
#define MONGOREADPREFSPROTOTYPE_H_DEF

#include <QObject>
#include <QScriptable>
#include <QScriptValue>
#include <qmongoreadprefs.h>

using porto::mongo::ReadPrefs;
class MongoReadPrefsPrototype : public QObject
			      , public QScriptable
{
  Q_OBJECT

public:
  explicit MongoReadPrefsPrototype (QObject *parent = nullptr);
  virtual ~MongoReadPrefsPrototype();

public slots:
  
private:
  ReadPrefs *self() const;
};


#endif // MONGOREADPREFSPROTOTYPE_H_DEF
