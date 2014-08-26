#ifndef SQLDATABASE_H_DEF
#define SQLDATABASE_H_DEF

#include <QObject>
#include <QScopedPointer>
class QScriptEngine;
class SqlDatabase : public QObject
{
   Q_OBJECT

public:
   SqlDatabase (QScriptEngine *engine, QObject *parent = nullptr);
   virtual ~SqlDatabase();

private:
   class Private;
   QScopedPointer <Private> d;
}; 

#endif
