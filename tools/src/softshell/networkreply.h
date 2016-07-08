#ifndef NETWORKREPLY_H_DEF
#define NETWORKREPLY_H_DEF

#include <QObject>
#include <QScopedPointer>

class QScriptEngine;
class NetworkReply : public QObject
{
  Q_OBJECT
public:
  explicit NetworkReply (QScriptEngine *object);
  virtual ~NetworkReply();

private:
  class Private;
  QScopedPointer <Private> d;

  NetworkReply (QObject *parent = nullptr);  
};

#endif // NETWORKREPLY_H_DEF
