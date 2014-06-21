#ifndef UDPSOCKET_H_DEF
#define UDPSOCKET_H_DEF

#include <QObject>

class QScriptEngine;
class UdpSocket : public QObject
{
   Q_OBJECT
public:
   UdpSocket(QScriptEngine *engine);
   virtual ~UdpSocket();

private:
   class Private;
   QScopedPointer<Private> d;

   UdpSocket (QObject *parent = nullptr);
};

#endif // UDPSOCKET_H_DEF
