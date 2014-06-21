#ifndef HTTPD_H_DEF
#define HTTPD_H_DEF

#include <QObject>
#include <QScopedPointer>

class QScriptEngine;
class Httpd : public QObject
{
  Q_OBJECT

public:
  explicit Httpd (QScriptEngine *object);
  virtual ~Httpd ();

private:
  class Private;
  QScopedPointer<Private> d;

  Httpd (QObject *parent = nullptr);
};

#endif
