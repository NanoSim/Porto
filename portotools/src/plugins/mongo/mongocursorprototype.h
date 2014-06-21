#ifndef MONGOCURSORPROTOTYPE_H_DEF
#define MONGOCURSORPROTOTYPE_H_DEF

#include <QObject>
#include <QScriptable>
#include <QScriptValue>
#include <qmongocursor.h>

using porto::mongo::Cursor;
class MongoCursorPrototype : public QObject
			   , public QScriptable
{
   Q_OBJECT

public:
   explicit MongoCursorPrototype (QObject *parent = nullptr);
   virtual ~MongoCursorPrototype();

public slots:
   bool more() const;
   bool error() const;
   bool isAlive() const;
   QScriptValue next();
   QScriptValue clone();
   QScriptValue current();
   
private:
   Cursor *self() const;
};

#endif // MONGOCURSORPROTOTYPE_H_DEF
