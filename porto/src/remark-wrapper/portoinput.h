#ifndef PORTOINPUT_H_DEF
#define PORTOINPUT_H_DEF

#include <QObject>
#include <collection.h>
class PortoInput : public QObject
{
  Q_OBJECT
public:
  PortoInput(soft::Collection *, QObject *parent = nullptr);
  virtual ~PortoInput();
                        
  public slots:
  void run();
  
signals:
  void finished();

private:
  soft::Collection *collection;
  
};

#endif // PORTOINPUT_H_DEF
