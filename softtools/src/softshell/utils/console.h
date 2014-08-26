#ifndef CONSOLE_H_DEF
#define CONSOLE_H_DEF

#include <QObject>

class QScriptEngine;
class Console : public QObject
{
  Q_OBJECT

public:
  explicit Console (QScriptEngine *obj);
  virtual ~Console();

private:
  Console (QObject *parent = nullptr);
};

#endif // CONSOLE_H_DEF
