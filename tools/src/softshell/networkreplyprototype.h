#ifndef NETWORKREPLYPROTOTYPE_H_DEF
#define NETWORKREPLYPROTOTYPE_H_DEF

#include <QScriptable>
#include <QObject>

class NetworkReplyPrototype : public QObject,
			      public QScriptable
{
  Q_OBJECT
public:
  explicit NetworkReplyPrototype(QObject *parent = nullptr);
  virtual ~NetworkReplyPrototype();

public slots:
  QScriptValue readAll();
  QScriptValue readBufferSize();
  QScriptValue waitForReadyRead(int);
};

#endif // NETWORKREPLYPROTOTYPE_H_DEF
