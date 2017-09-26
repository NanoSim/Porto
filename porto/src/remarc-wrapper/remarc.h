#pragma once
#include <QObject>
#include <collection.h>

class QProcess;
class Remarc : public QObject
{
  Q_OBJECT
  
public:  
  Remarc(soft::Collection *, QString remarcPath, QObject *parent = nullptr);
  virtual ~Remarc();

public slots:
  void run();
  void exited();
  void started();
  void readStdout();
  void readStderr();

signals:
  void finished();
  
private:
  QString remarcPath;
  QProcess *process;
  soft::Collection *collection;
};
