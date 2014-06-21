#ifndef APPLICATIONPROTOTYPE_H_DEF
#define APPLICATIONPROTOTYPE_H_DEF

#include <QScriptable>
#include <QObject>
#include <QScriptValue>

class ApplicationPrototype : public QObject,
			     public QScriptable
{
  Q_OBJECT
public:
  explicit ApplicationPrototype (QObject * parent = 0);
  virtual ~ApplicationPrototype ();

public slots:
  QScriptValue exec();
}; // class ApplicationPrototype
  

#endif // APPLICATIONPROTOTYPE_H_DEF
