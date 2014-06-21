#include <QtCore>
#include <QtNetwork>
#include <QScriptEngine>

#include "udpsocketprototype.h"
#include "udpsocket.h"
#include "protocall.h"

UdpSocketPrototype :: UdpSocketPrototype (QObject *parent)
  : QObject (parent)
{
  auto socket = qscriptvalue_cast <QUdpSocket*> (thisObject());
  if (socket) {
    connect (socket, SIGNAL(readyRead()), SIGNAL(readyRead()));
  }
}

UdpSocketPrototype :: ~UdpSocketPrototype()
{}

QScriptValue UdpSocketPrototype :: hasPendingDatagrams() const
{
  return protocall <QUdpSocket> (thisObject(), this->parent(),
				 [](QUdpSocket *socket)
				 -> bool
    {
      return socket->hasPendingDatagrams();
    });
}

QScriptValue UdpSocketPrototype :: bind (QString const &hostAddress,
					 quint16 port)
{
  return protocall <QUdpSocket> (thisObject(), this->parent(),
				 [=](QUdpSocket *socket)
				 -> bool
    {
      QHostAddress hostadr (hostAddress);
      return socket->bind (hostadr, port);
    });
}

QScriptValue UdpSocketPrototype :: readDatagram ()
{
  return protocall_qscriptvalue <QUdpSocket> (thisObject(),
					      [=](QUdpSocket *socket)
					      -> QScriptValue
    {
      QByteArray buffer;
      QHostAddress sender;
      quint16 senderPort;
      buffer.resize (socket->pendingDatagramSize());
      socket->readDatagram (buffer.data(), buffer.size(), &sender, &senderPort);

      QScriptValue bufferScriptValue (buffer.constData());
      QScriptValue senderScriptValue (sender.toString());
      QScriptValue senderPortScriptValue (senderPort);
      auto obj = engine()->newObject();
      obj.setProperty ( QString("datagram"),   bufferScriptValue);
      obj.setProperty ( QString("sender"),     senderScriptValue);
      obj.setProperty ( QString("senderPort"), senderPortScriptValue);

      return obj;
    });
}

QScriptValue UdpSocketPrototype :: writeDatagram (QString const &datagram, 
						  QString const &hostAddress, 
						  quint16 port)
{
  return protocall <QUdpSocket> (thisObject(), this->parent(),
				 [=] (QUdpSocket *udpSocket)
				 -> qint64
    {
      QHostAddress hostadr (hostAddress);
      return udpSocket->writeDatagram (datagram.toLocal8Bit(), hostadr, port); 
    });
}
