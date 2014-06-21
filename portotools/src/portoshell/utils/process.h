#ifndef PROCESS_H_DEF
#define PROCESS_H_DEF

#include <QObject>
#include <QScopedPointer>

class QScriptEngine;
class Process : public QObject
{
   Q_OBJECT

public:
   explicit Process (QScriptEngine *obj);
   virtual ~Process();

private:
   class Private;
   QScopedPointer<Private> d;

   Process (QObject *parent = nullptr);
};

#endif // PROCESS_H_DEF
