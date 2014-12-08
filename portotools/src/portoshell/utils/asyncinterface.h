#ifndef ASYNCINTERFACE_H_DEF
#define ASYNCINTERFACE_H_DEF

#include <QObject>
#include <QScopedPointer>
class QScriptEngine;
class AsyncInterface : public QObject
{
  Q_OBJECT

public:
  explicit AsyncInterface (QScriptEngine *engine);
  virtual ~AsyncInterface();

private:
  class Private;
  QScopedPointer <Private> d;

  AsyncInterface (QObject *parent = nullptr);
};

#endif // ASYNCINTERFACE_H_DEF
