#ifndef EVENTLOOP_H_DEF
#define EVENTLOOP_H_DEF

#include <QObject>
#include <QScopedPointer>

class QScriptEngine;
class EventLoop : public QObject
{
  Q_OBJECT

public:
  explicit EventLoop (QScriptEngine *object);
  virtual ~EventLoop();

private:
  class Private;
  QScopedPointer<Private> d;

  EventLoop (QObject *parent = nullptr);
};

#endif // EVENTLOOP_H_DEF
