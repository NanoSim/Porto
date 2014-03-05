#ifndef SYS_H_DEF
#define SYS_H_DEF

#include <QScriptValue>
#include <QString>

class Sys
{  
  QScriptValue &callback;
 public:
  Sys(QScriptValue &callback);
  QScriptValue exec(QString const & cmd);
};

#endif
