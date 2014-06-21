#ifndef APPLICATION_H_DEF
#define APPLICATION_H_DEF

#include <QObject>
#include <QScopedPointer>

class QScriptEngine;
class Application : public QObject
{
   Q_OBJECT

public:
   explicit Application (QScriptEngine *obj);
   virtual ~Application();

private:
   class Private;
   QScopedPointer<Private> d;

   Application(QObject *parent = nullptr);
};

#endif // APPLICATION_H_DEF
