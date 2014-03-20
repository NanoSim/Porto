#include "sys.h"
#include <QProcess>


Sys :: Sys(QScriptValue &callback)
  : callback(callback)
{
  
}

QScriptValue Sys :: exec(QString const & cmd) 
{
  // perform task.
  QProcess proc;
  proc.start(cmd);

  return QScriptValue(proc.waitForFinished(-1));
}
