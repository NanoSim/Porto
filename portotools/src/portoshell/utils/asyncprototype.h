#ifndef ASYNCPROTOTYPE_H_DEF
#define ASYNCPROTOTYPE_H_DEF

#include <QScriptable>
#include <QObject>
#include <QScriptValue>

class AsyncPrototype : public QObject
		     , public QScriptable
{
  Q_OBJECT

public:
  explicit AsyncPrototype (QObject *parent = nullptr);
  virtual ~AsyncPrototype();
};

#endif // ASYNCPROTOTYPE_H_DEF
