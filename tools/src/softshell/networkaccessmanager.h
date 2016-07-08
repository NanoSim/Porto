#ifndef NETWORKACESSMANAGER_H_DEF
#define NETWORKACESSMANAGER_H_DEF

#include <QObject>
#include <QScopedPointer>

class QScriptEngine;
class NetworkAccessManager : public QObject
{
  Q_OBJECT
public:
  explicit NetworkAccessManager (QScriptEngine *object);
  virtual ~NetworkAccessManager();

private:
  class Private;
  QScopedPointer <Private> d;

  NetworkAccessManager (QObject *parent = nullptr);  
};

#endif // NETWORKACESSMANAGER_H_DEF
