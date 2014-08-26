#include <QtCore>
#include <QScriptEngine>
#include "processprototype.h"
#include "protocall.h"

ProcessPrototype :: ProcessPrototype (QObject *parent)
  : QObject(parent)
{   
  QProcess *process = qscriptvalue_cast <QProcess*> (thisObject ());
  if(process) {
    connect (process, SIGNAL (finished ()),                SIGNAL (finished ()));
    connect (process, SIGNAL (readyReadStandardError ()),  SIGNAL (readyReadStandardError ()));
    connect (process, SIGNAL (readyReadStandardOutput ()), SIGNAL (readyReadStandardOutput ()));
    connect (process, SIGNAL (started ()),                 SIGNAL (started ()));
  }
}

ProcessPrototype :: ~ProcessPrototype()
{}

QScriptValue ProcessPrototype :: readAllStandardError()
{
  return protocall<QProcess>(thisObject(), this->parent(), 
			     [](QProcess *process) 
			     -> QByteArray 
    {
      return process->readAllStandardError();
    });
}

QScriptValue ProcessPrototype :: readAllStandardOutput()
{
  return protocall<QProcess>(thisObject(), this->parent(), 
			     [](QProcess *p) 
			     -> QByteArray 
    {
      return p->readAllStandardOutput();
    });
}

QScriptValue ProcessPrototype :: execute (QString const &program)
{
  return protocall<QProcess>(thisObject(), this->parent(),
			     [&](QProcess *) 
			     -> int 
    {
      return (QProcess::execute(program));
    });
}

QScriptValue ProcessPrototype :: arguments() const
{
  return protocall_sequence<QProcess>(thisObject(), this->parent(),
				      [](QProcess *process) 
				      -> QStringList
    {
      return process->arguments();
    });
}

QScriptValue ProcessPrototype :: setArguments(QStringList const & arguments)
{
  return protocall_void<QProcess>(thisObject(), this->parent(),
				  [=](QProcess *process) 
				  -> void
    {
      process->setArguments(arguments);
    });  
}

QScriptValue ProcessPrototype :: program() const
{
  return protocall<QProcess>(thisObject(), this->parent(),
			     [](QProcess *p) 
			     -> QString
    {
      return p->program();
    });  
}

QScriptValue ProcessPrototype :: setProgram(QString const & program)
{
  return protocall_void<QProcess>(thisObject(), this->parent(),
				  [=](QProcess *p) 
				  -> void
    {
      p->setProgram(program);
    });
}


QScriptValue ProcessPrototype :: start()
{
  return protocall_void<QProcess>(thisObject(), this->parent(),
				  [](QProcess *p) 
				  -> void
    {
      p->start();
    });
}

QScriptValue ProcessPrototype :: waitForFinished(int msecs)
{
  return protocall<QProcess>(thisObject(), this->parent(),
			     [=](QProcess *p) 
			     -> bool 
    {
      return p->waitForFinished(msecs);
    });
}
