#ifndef HTTPDPROTOTYPE_H_DEF
#define HTTPDPROTOTYPE_H_DEF

#include <QScriptable>
#include <QObject>
#include <QScriptValue>

class HttpdPrototype : public QObject
		     , public QScriptable
{
  Q_OBJECT

public:
  explicit HttpdPrototype (QObject *parent = nullptr);
  virtual ~HttpdPrototype();

public slots:
  QScriptValue start();
  QScriptValue stop();
  QScriptValue setRootDir(QString const &path);
};
#endif // HTTPDPROTOTYPE_H_DEF
