#ifndef ASYNC_H_DEF
#define ASYNC_H_DEF

#include <QObject>
#include <QScopedPointer>
#include <QScriptValue>

class QScriptEngine;
//class QScriptValue;
class Async : public QObject
{
  Q_OBJECT

public:
  explicit Async (QScriptEngine *engine);
  virtual ~Async();

public slots:
  void eval(QScriptValue fn);

private slots:
  void asyncEval(QScriptValue fn);
  void processQueue();

signals:
  void asyncRequest(QScriptValue fn);
  void result(QScriptValue const &res);
  
private:
  class Private;
  QScopedPointer<Private> d;
  Async (QObject *parent = nullptr);
  
};

#endif //ASYNC_H_DEF
