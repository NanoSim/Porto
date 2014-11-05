#include <QtCore>
#include <QScriptEngine>
#include "procsprototype.h"
#include "protocall.h"

ProcessPrototype :: ProcessPrototype (QObject *parent)
  : QObject(parent)
{   
  QProcess *process = qscriptvalue_cast <QProcess*> (thisObject ());
  if(process) {
    connect (process, SIGNAL (finished (int, QProcess::ExitStatus)),                SIGNAL (finished (int, int)));
    connect (process, SIGNAL (error(QProcess::ProcessError)), SIGNAL (error(int)));
    connect (process, SIGNAL (readyReadStandardError ()),  SIGNAL (readyReadStandardError ()));
    connect (process, SIGNAL (readyReadStandardOutput ()), SIGNAL (readyReadStandardOutput ()));
    connect (process, SIGNAL (started ()),                 SIGNAL (started ()));
    connect (process, SIGNAL (error (QProcess::ProcessError)), SIGNAL (error (int)));
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

QScriptValue ProcessPrototype :: execute (QString const &program, QStringList const &args)
{
  return protocall<QProcess>(thisObject(), this->parent(),
			     [&](QProcess *) 
			     -> int 
    {
      return (QProcess::execute(program, args));
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

QScriptValue ProcessPrototype :: waitForStarted(int msecs)
{
  return protocall<QProcess>(thisObject(), this->parent(),
			     [=](QProcess *p) 
			     -> bool 
    {
      return p->waitForStarted(msecs);
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

QScriptValue ProcessPrototype :: kill()
{
  return protocall_void<QProcess>(thisObject(), this->parent(),
				  [](QProcess *p)
				  -> void
    {
      return p->kill();
    });
}

QScriptValue ProcessPrototype :: isRunning()
{
  return protocall<QProcess>(thisObject(), this->parent(),
			     [](QProcess *p)
			     -> bool
    {
      return p->state() == QProcess::Running;
    });

}

QScriptValue ProcessPrototype :: isNotRunning()
{
  return protocall<QProcess>(thisObject(), this->parent(),
			     [](QProcess *p)
			     -> bool
    {
      return p->state() == QProcess::NotRunning;
    });
}

QScriptValue ProcessPrototype :: isStarting()
{
  return protocall<QProcess>(thisObject(), this->parent(),
			     [](QProcess *p)
			     -> bool
    {
      return p->state() == QProcess::Starting;
    });
}


QScriptValue ProcessPrototype :: write (QString const &buffer)
{
  return protocall<QProcess>(thisObject(), this->parent(),
			     [buffer](QProcess *p)
			     -> qint64
    {
      return p->write(qPrintable(buffer));
    });			    
}

QScriptValue ProcessPrototype :: writeln (QString const &buffer)
{
  return protocall<QProcess>(thisObject(), this->parent(),
			     [buffer](QProcess *p)
			     -> qint64
    {
      QByteArray ba;
      QTextStream stream(&ba, QIODevice::WriteOnly);
      stream << buffer << endl;
      return p->write(ba.constData());
    });			    
}

QScriptValue ProcessPrototype :: closeReadChannel(ProcessChannel &channel)
{
  return protocall_void<QProcess>(thisObject(), this->parent(),
				  [channel](QProcess *process)
				  -> void
    {
      process->closeReadChannel((QProcess::ProcessChannel)channel);
    });
}

QScriptValue ProcessPrototype :: closeWriteChannel()
{
  return protocall_void<QProcess>(thisObject(), this->parent(),
				  [](QProcess *process)
				  -> void
    {
      process->closeWriteChannel();
    });
}

