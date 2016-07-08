#ifndef CONCURRENT_H_DEF
#define CONCURRENT_H_DEF

#include <QObject>
#include <QScopedPointer>

class QScriptEngine;
class Concurrent : public QObject
{
  Q_OBJECT

public:
  explicit Concurrent (QScriptEngine *object);
  virtual ~Concurrent();

private:
  class Private;
  QScopedPointer<Private> d;

  Concurrent (QObject *parent = nullptr);
}; // class Concurrent

#endif // CONCURRENT_H_DEF
