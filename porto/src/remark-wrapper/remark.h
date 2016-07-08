#pragma once
#include <QObject>
#include <collection.h>

class QProcess;
class Remark : public QObject
{
  Q_OBJECT
  
public:  
  Remark(soft::Collection *, QObject *parent = nullptr);
  virtual ~Remark();

public slots:
  void run();
  void exited();
  void started();
  void readStdout();
  void readStderr();

signals:
  void finished();
  
private:
  QProcess *process;
  soft::Collection *collection;
};
