#ifndef CONSOLE_H_DEF
#define CONSOLE_H_DEF

#include <QObject>
class Console : public QObject
{
  Q_OBJECT

public:
  Console (QObject *parent = nullptr);
  virtual ~Console();

public slots:
  void write (QString const &data);

private:
  class Private;
  Private *d;
};
#endif // CONSOLE_H_DEF
