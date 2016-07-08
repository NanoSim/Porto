#include <QtCore>
#include <QScriptEngine>
#include <QScriptValue>
#include <QEventLoop>
#include "eventloopprototype.h"
#include "protocall.h"

EventLoopPrototype :: EventLoopPrototype (QObject *parent)
  : QObject(parent)
{}

EventLoopPrototype :: ~EventLoopPrototype ()
{}

QScriptValue EventLoopPrototype :: exec ()
{
  return protocall<QEventLoop> (thisObject(), this->parent(), 
				[] (QEventLoop *eventLoop) -> int {
				  return eventLoop->exec();
				});
}

QScriptValue EventLoopPrototype :: exit (int returnCode)
{
  return protocall_void<QEventLoop> (thisObject(), this->parent(),
				     [=] (QEventLoop *eventLoop) 
				       -> void
    {
      eventLoop->exit(returnCode);
    });
}


QScriptValue EventLoopPrototype :: isRunning ()
{
  return protocall_void<QEventLoop> (thisObject(), this->parent(),
				[] (QEventLoop *eventLoop) {
				  eventLoop->isRunning();
				});
}

QScriptValue EventLoopPrototype :: wakeUp ()
{
  return protocall_void<QEventLoop> (thisObject(), this->parent(),
				[] (QEventLoop *eventLoop) {
				  eventLoop->wakeUp();
				});
}

QScriptValue EventLoopPrototype :: quit ()
{
  return protocall_void<QEventLoop> (thisObject(), this->parent(),
				[] (QEventLoop *eventLoop) {
				  eventLoop->quit();
				});
}
