#include <functional>
#include <QProcess>
#include <QTcpSocket>
#include <QDateTime>
#include <QUrl>
#include <QFile>
#include <QFileInfo>
#include <QScriptEngine>
#include <QScriptContext>
#include <QScriptValue>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QDir>
#include "httpdaemon.h"

static constexpr auto settings_conf          = "configure";
static constexpr auto settings_directory_index = "directory_index";
static constexpr auto settings_types         = "types";
static constexpr auto settings_port          = "port";
static constexpr auto settings_document_root = "document_root";
static constexpr auto settings_server_name   = "server_name";
static constexpr auto settings_server_admin  = "server_admin";
static constexpr auto settings_error_log     = "error_log";
static constexpr auto settings_action        = "actions";
static constexpr auto settings_handler       = "handler";

static QScriptValue action(QScriptContext *context, QScriptEngine *engine)
{
  auto key      = context->argument(0).toString();
  auto value    = context->argument(1);
  auto global   = engine->globalObject();
  auto settings = global.property(settings_conf);
  auto action   = settings.property(settings_action);
  action.setProperty(key, value);
  return engine->undefinedValue();
}

static QScriptValue addHandler(QScriptContext *context, QScriptEngine *engine)
{
  auto key      = context->argument(0).toString();
  auto value    = context->argument(1);
  auto global   = engine->globalObject();
  auto settings = global.property(settings_conf);
  auto handler  = settings.property(settings_handler);
  handler.setProperty(key, value);
  return engine->undefinedValue();
}

static QScriptValue setDocumentRoot(QScriptContext *context, QScriptEngine *engine)
{
  auto root     = context->argument(0).toString();
  auto global   = engine->globalObject();
  auto settings = global.property(settings_conf);
  settings.setProperty(settings_document_root, root);
  return engine->undefinedValue();
}

static QScriptValue setServerName(QScriptContext *context, QScriptEngine *engine)
{
  auto name     = context->argument(0).toString();
  auto global   = engine->globalObject();
  auto settings = global.property(settings_conf);
  settings.setProperty(settings_server_name, name);
  return engine->undefinedValue();
}

static QScriptValue setServerAdmin(QScriptContext *context, QScriptEngine *engine)
{ 
  auto arg      = context->argument(0).toString();
  auto global   = engine->globalObject();
  auto settings = global.property(settings_conf);
  settings.setProperty(settings_server_admin, arg);
  return engine->undefinedValue();
}


static QScriptValue setErrorLog(QScriptContext *context, QScriptEngine *engine)
{
  auto arg      = context->argument(0).toString();
  auto global   = engine->globalObject();
  auto settings = global.property(settings_conf);
  settings.setProperty(settings_error_log, arg);
  return engine->undefinedValue();
}

static QScriptValue setDirectoryIndex(QScriptContext *context, QScriptEngine *engine)
{
  qDebug() << "QScriptValue setDirectoryIndex";
  auto directoryIndex = context->argument(0).toString();
  qDebug() << "\t" << directoryIndex;
  auto global   = engine->globalObject();   
  auto settings = global.property(settings_conf);
  settings.setProperty(settings_directory_index, directoryIndex);
  return  engine->undefinedValue();
}

static QScriptValue setPort (QScriptContext *context, QScriptEngine *engine)
{
  auto port     = context->argument(0).toUInt16();
  auto global   = engine->globalObject();   
  auto settings = global.property(settings_conf);
  settings.setProperty(settings_port, port);
  return  engine->undefinedValue();
     
}

static QScriptValue addType (QScriptContext *context, QScriptEngine *engine)
{
  auto key      = context->argument(0).toString();
  auto value    = context->argument(1);
  auto global   = engine->globalObject();
  auto settings = global.property(settings_conf);
  auto types    = settings.property(settings_types);

  types.setProperty(key, value);
  
  return engine->undefinedValue();
}

static QScriptValue addRequestHeader (QScriptContext *context, QScriptEngine *engine)
{
  Q_UNUSED(context);
  return engine->undefinedValue();
}


class HttpDaemon :: Private
{
  friend class HttpDaemon;
  QString configScript;
  QJsonDocument serverSettings;

  QString directoryIndex() const;
  quint16 port() const;
  QString findType(QString const &filename) const;
  QString findHandler(QString const &filename) const;
  QString documentRoot() const;
  QByteArray performAction(QString const &handler, QString const &filename, QString const &query) const;
  
  bool initialize();
};

QString HttpDaemon::Private::directoryIndex() const
{
  auto idx = serverSettings.object().value(settings_directory_index).toString();
  qDebug() << "HttpDaemon::Private::directoryIndex" << idx;
    
  return idx;
}

quint16 HttpDaemon::Private::port() const
{
  return serverSettings.object().value(settings_port).toInt(8080);
}

QString HttpDaemon::Private::documentRoot() const
{
  auto root = serverSettings.object().value(settings_document_root).toString();
  return root;
}

QByteArray HttpDaemon::Private::performAction(QString const &handler, QString const &filename, QString const &query) const
{
  auto action = serverSettings.object().value(settings_action);
  auto actionObj = action.toObject();
  auto actionScript = actionObj.value(handler).toString();

  qDebug() << "-----------------HttpDaemon::Private::performAction-----------------------------";
  qDebug() << "action: " << actionScript;
  qDebug() << " handler: " << handler;

  QProcess proc;
  QString command = actionScript;
  qDebug() << "command: " << command;
  QStringList args = QStringList() << filename << query;
  qDebug() << "args: " << args;
  proc.start(command, args);
  proc.waitForFinished();
 
  if (proc.exitStatus() != QProcess::NormalExit) {
    return QByteArray();
  }

  qDebug() << proc.readAllStandardError();
  auto retval = proc.readAllStandardOutput();
  return retval;
}

QString HttpDaemon::Private::findHandler(QString const &filename) const
{
  QFileInfo fileInfo(filename);
  auto suffix = fileInfo.suffix();
  auto handle = serverSettings.object().value(settings_handler);
  auto handleObj = handle.toObject();
  for (auto k : handleObj.keys()) {
    if (handleObj.value(k).toArray().contains(suffix))
      return k;
  }
  return QString();
}

QString HttpDaemon::Private::findType(QString const &filename) const
{
  QFileInfo fileInfo(filename);
  auto suffix = fileInfo.suffix();
  auto types = serverSettings.object().value(settings_types);
  auto typesObj = types.toObject();
  for (auto k : typesObj.keys()) {
    if (typesObj.value(k).toArray().contains(suffix))
      return k;
  }
  return QString();
}

bool HttpDaemon::Private::initialize()
{
  QScriptEngine engine;
   
  QFile file(configScript);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qDebug() << file.errorString();
    return false;
  }

  QTextStream stream (&file);

  auto globalObject = engine.globalObject();
  auto configObject = engine.newObject();
  auto typesObject  = engine.newObject();
  auto actionObject = engine.newObject();
  auto handlerObject= engine.newObject();
  auto addTypeFn    = engine.newFunction(addType);
  auto setPortFn    = engine.newFunction(setPort);
  auto addReqHdrFn  = engine.newFunction(addRequestHeader);
  auto setDocRootFn = engine.newFunction(setDocumentRoot);
  auto setServNamFn = engine.newFunction(setServerName);
  auto setServAdmFn = engine.newFunction(setServerAdmin);
  auto setErrLogFn  = engine.newFunction(setErrorLog);
  auto setIndexDefFn= engine.newFunction(setDirectoryIndex);
  auto actionFn     = engine.newFunction(action);
  auto addHandlerFn = engine.newFunction(addHandler);
  
  configObject.setProperty(settings_types, typesObject);
  configObject.setProperty(settings_action, actionObject);
  configObject.setProperty(settings_handler, handlerObject);
  globalObject.setProperty(settings_conf, configObject);
  configObject.setProperty("action", actionFn);
  configObject.setProperty("addHandler", addHandlerFn);
  configObject.setProperty("addType", addTypeFn);
  configObject.setProperty("setListenPort", setPortFn);
  configObject.setProperty("addRequestHeader", addReqHdrFn);
  configObject.setProperty("setDocumentRoot", setDocRootFn);
  configObject.setProperty("setServerName", setServNamFn);
  configObject.setProperty("setServerAdmin", setServAdmFn);
  configObject.setProperty("setErrorLog", setErrLogFn);
  configObject.setProperty("setDirectoryIndex", setIndexDefFn);
  
  auto const program = stream.readAll();
  auto const result = engine.evaluate(program, file.fileName(), 1);
  if (engine.hasUncaughtException()) {
    auto line = engine.uncaughtExceptionLineNumber();
    qDebug() << "uncaught exception at line" << line << ":" << result.toString();
    return false;
  }
  auto configValue = engine.evaluate("(function(){return (JSON.stringify(configure));})();");
  serverSettings = QJsonDocument::fromJson(configValue.toString().toLocal8Bit());
  return true;
}

HttpDaemon :: HttpDaemon(QString const &configScript, QObject *parent)
  : QTcpServer(parent)
  , d (new HttpDaemon::Private())
{
  d->configScript = configScript;
}

HttpDaemon :: ~HttpDaemon()
{
  delete d;
}

bool HttpDaemon :: run()
{
  auto isOk = d->initialize();
  if (isOk) {
    if (!listen (QHostAddress::Any, d->port())) {
      qWarning() << "Cannot listen to port" << d->port();
    }
  }
  return isOk;
}

void HttpDaemon :: incomingConnection (qintptr socket)
{
  QTcpSocket *s = new QTcpSocket(this);
  connect (s, SIGNAL (readyRead()), this, SLOT (readClient()));
  connect (s, SIGNAL (disconnected()), this, SLOT (discardClient()));
  s->setSocketDescriptor (socket);
}

QByteArray readFile(QString const &filename)
{  
  QFile file(filename);
  if (!file.open (QIODevice::ReadOnly)) {
    return QByteArray();
  }
  return file.readAll();    
}

void HttpDaemon :: handleToken(QStringList const &token, QTcpSocket *socket)
{
  if (token[0] == "GET") {
    auto tok = (token[1].trimmed().right(1) == "/" ?
		(token[1].trimmed() + d->directoryIndex()) :
		token[1].trimmed());
    QUrl url("file://" + tok);
    auto fileName = d->documentRoot() + url.path();
    auto handler = d->findHandler(fileName);

    qDebug() << "--------------------------HttpDaemon :: handleToken----------------------";
    qDebug() << "  filename:" << fileName;
    qDebug() << "  handler:" << handler;
    qDebug() << "  url:" << QUrl::fromPercentEncoding(url.toEncoded());
    qDebug() << "  header\n" << qPrintable(socket->readAll());
      
    QByteArray data;
    if (!handler.isEmpty()) {
      data = d->performAction(handler, fileName, url.query());
      qDebug() << data;
      if (data.isEmpty()) {
	socket->write("HTTP/1.1 404 Not found\r\n");
      } else {
	qDebug() << "#### HANDLING ACTION!";
	//	socket->write("HTTP/1.1 200 OK\r\n");
	//	socket->write("Connection: close\r\n");
	socket->write(data);
      }
    } else {      
      auto type = d->findType(fileName);     
      auto data = readFile(fileName);
      if (data.isEmpty()) {
	socket->write("HTTP/1.1 404 Not found\r\n");
      } else {
	socket->write("HTTP/1.1 200 OK\r\n");
      }
      
      socket->write("Connection: close\r\n");
      socket->write(qPrintable(QString("Date: %1\r\n").arg(QString(QDateTime::currentDateTime().toString()))));
      socket->write(qPrintable(QString("Expires: %1\r\n").arg(QString(QDateTime::currentDateTime().toString()))));
      socket->write(qPrintable(QString("Content-Type: %1\r\n").arg(type)));
      socket->write(qPrintable(QString("Content-Length: %1\r\n").arg(QString::number(data.size()))));
      socket->write( "\r\n");
      socket->write(data);
    }

    socket->close();
  } else if (token[0] == "POST") {
    qDebug() << "--------------------------   POST  ----------------------";
    qDebug() << "token = " << token;
    qDebug() << socket->readAll();
  }
}

void HttpDaemon :: discardClient()
{
  QTcpSocket *socket = (QTcpSocket*)sender();
  socket->deleteLater();
}

void HttpDaemon :: readClient()
{
  QTcpSocket *socket = (QTcpSocket*) sender();
  if (socket->canReadLine()) {
    QStringList tokens = QString(socket->readLine()).split(QRegExp("[ \r\n][ \r\n]*"));
    qDebug() << "reading token: " << tokens.join(",");
    handleToken(tokens, socket);
    if (socket->state() == QTcpSocket::UnconnectedState) {
      delete socket;
    }
  }
}
