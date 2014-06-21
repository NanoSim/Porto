#include "httpserver.h"
#include <QTextStream>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QRegExp>
#include <QDateTime>
#include <QDir>
#include <QFile>

class HttpServer::Private
{
  friend class HttpServer;
  Private(quint16 port)
    : port(port)
    , path(QDir::current())
  {}

  quint16 port;
  QDir path;
};

HttpServer :: HttpServer (quint16 port, QObject *parent)
  : QTcpServer (parent)
  , d (new HttpServer::Private(port))
{
}

HttpServer :: HttpServer(HttpServer const &other)
  : QTcpServer (other.parent())
  , d (new HttpServer::Private(other.d->port))
{
}

HttpServer :: ~HttpServer()
{}

quint16 HttpServer :: port() const
{
  return d->port;
}

bool HttpServer :: start()
{
  listen (QHostAddress::Any, d->port);
  QTextStream(stdout) << "[httpd started on port " << d->port << "]" << endl;

  return true;
}

bool HttpServer :: stop()
{
  return true;
}

void HttpServer :: incomingConnection(qintptr socketDescriptor)
{
  auto socket = new QTcpSocket (this);
  connect (socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
  connect (socket, SIGNAL(disconnected()), this, SLOT(disconnectSocket()));
  socket->setSocketDescriptor(socketDescriptor);
}

void HttpServer :: replyGet(QTcpSocket *socket, QString const &req)
{
  QTextStream os (socket);
  os.setAutoDetectUnicode (true);
  QDir const dir (d->path.path() + req);
  auto contentFile = dir.canonicalPath();
  QFileInfo const info(contentFile);
  
  if (!info.exists() || !info.isFile() || !info.isReadable()) {
    os << "HTTP/1.0 404 Not Found\r\n"
      "\r\n";
  } else {
    
    auto size = QString::number(info.size());
    auto lastModified = info.lastModified().toString();
    auto suffix  = info.suffix();

    QFile file(contentFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      os << "HTTP/1.0 404 Not Found\r\n"
	"\r\n";
    }
    
    os << "HTTP/1.1 200 Ok\r\n"
      "Date: " << QDate::currentDate().toString() << "\r\n"
      "Last-Modified: " << lastModified << "\r\n"
      "Content-Type: text/html; charset=\"UTF-8\"\r\n"
      "Content-Length: " << size << "\r\n"
      "\r\n";
      
    QTextStream in(&file);
    while (!in.atEnd()) {
      os << in.readLine() << "\n";
    }
  }
  
  socket->close();

  if (socket->state() == QTcpSocket::UnconnectedState) {
    delete socket;
  }
}

void HttpServer :: readSocket()
{
  auto socket = qobject_cast<QTcpSocket*>(sender());
  Q_CHECK_PTR(socket);
  if (socket->canReadLine()) {
    auto tokens = QString (socket->readLine()).split(QRegExp("[ \r\n][ \r\n]*"));
    if (tokens[0] == "GET") replyGet(socket, tokens[1]);
  }
}

void HttpServer :: disconnectSocket()
{
  auto socket = qobject_cast<QTcpSocket*>(sender());
  Q_CHECK_PTR(socket);
  socket->deleteLater();
}

bool HttpServer :: setRootDir (QString const &path)
{
  d->path.setPath(path);
  return d->path.exists();
}
