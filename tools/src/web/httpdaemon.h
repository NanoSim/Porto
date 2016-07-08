#pragma once
#include <QTcpServer>

/*!
  \class HttpDaemon
  \ingroup soft-tools
  \brief The HttpDaemon class defines a simple web-server

  \todo {add more comments here}
 */
class HttpDaemon : public QTcpServer
{
  Q_OBJECT

public:
  HttpDaemon(QString const &configScript, QObject *parent = 0);
  virtual ~HttpDaemon();
  bool run();
  
protected:
  void incomingConnection (qintptr socket) override;

private slots:
  void readClient();
  void discardClient();
    
private:
  void handleToken(QStringList const &token, QTcpSocket *socket);

  class Private;
  Private *d;
};
