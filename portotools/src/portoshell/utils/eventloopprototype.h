#ifndef EVENTLOOPPROTOTYPE_H_DEF
#define EVENTLOOPPROTOTYPE_H_DEF

#include <QScriptable>
#include <QObject>
#include <QScriptValue>

class EventLoopPrototype : public QObject,
			   public QScriptable
{
  Q_OBJECT
public:
  explicit EventLoopPrototype (QObject *parent = 0);
  virtual ~EventLoopPrototype();

public slots:
  QScriptValue exec();
  QScriptValue exit(int returnCode = 0);
  QScriptValue isRunning();
  QScriptValue wakeUp();
  QScriptValue quit();

}; // class EventLoopPrototype

#endif // EVENTLOOPPROTOTYPE_H_DEF
