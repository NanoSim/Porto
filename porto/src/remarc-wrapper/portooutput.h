#ifndef PORTOOUTPUT_H_DEF
#define PORTOOUTPUT_H_DEF

#include <QObject>
#include <collection.h>

class PortoOutput : public QObject
{
  Q_OBJECT
public:
  PortoOutput(soft::Collection *, QObject *parent = nullptr);
  virtual ~PortoOutput();

public slots:
  void run();

signals:
  void finished();

private:
  soft::Collection *collection;
};

#endif // PORTOOUTPUT_H_DEF
