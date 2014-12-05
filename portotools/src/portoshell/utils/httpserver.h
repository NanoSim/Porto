#ifndef HTTPSERVER_H_DEF
#define HTTPSERVER_H_DEF

#include <QObject>
#include <QTcpServer>

class QString;
class QTcpSocket;
class HttpServer : public QTcpServer
{
  Q_OBJECT
public:
  HttpServer (quint16 port = 8080, QObject *parent = nullptr);
  HttpServer(HttpServer const &other);
  virtual ~HttpServer();

  quint16 port() const;
  bool start();
  bool stop();
  bool setRootDir(QString const &path);

protected:
  //  virtual void addPendingConnection(QTcpSocket * socket);
  virtual void incomingConnection(qintptr socketDescriptor);
  void replyGet(QTcpSocket *socket, QString const & req);
		 
private slots:
  void readSocket();
  void disconnectSocket();

private:
  class Private;
  QScopedPointer<Private> d;
};

#endif // HTTPSERVER_H_DEF
