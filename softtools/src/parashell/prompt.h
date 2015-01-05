#ifndef PROMPT_H_DEF
#define PROMPT_H_DEF

#include <QObject>
class Prompt : public QObject
{
  Q_OBJECT

public:
  Prompt (QObject *parent = nullptr);
  virtual ~Prompt();

public slots:
  void start();
		   
protected slots:
  void read();

public slots:
  QString getMessage();

signals:
  void ready (QString const &buffer);

private:
  class Private;
  Private * d;
};
#endif // PROMPT_H_DEF
