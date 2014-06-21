#ifndef UDPSOCKETPROTOTYPE_H_DEF
#define UDPSOCKETPROTOTYPE_H_DEF

#include <QScriptable>
#include <QObject>
#include <QScriptValue>
#include <QString>
#include <QByteArray>

class UdpSocketPrototype : public QObject
			 , public QScriptable
{
  Q_OBJECT
  
public:
  explicit UdpSocketPrototype (QObject *parent = nullptr);
  virtual ~UdpSocketPrototype();

public slots:
  QScriptValue bind (QString const &hostAddress,
		     quint16 port);
  QScriptValue hasPendingDatagrams() const;
  QScriptValue readDatagram ();
  QScriptValue writeDatagram (QString const &datagram, 
			      QString const &hostAddress, 
			      quint16 port);
signals:
  void readyRead();
};

#endif // UDPSOCKETPROTOTYPE_H_DEF
