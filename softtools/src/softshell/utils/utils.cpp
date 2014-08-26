#include <Porto>
#include <QScriptValue>
#include <QScriptEngine>
#include "utils.h"
#include "process.h"
#include "application.h"
#include "eventloop.h"
#include "udpsocket.h"
#include "console.h"
#include "filesystem.h"
#include "filesystemwatcher.h"
#include "httpd.h"
using porto::ScriptEngine;

class Utils::Private
{
   friend class Utils;
   Private(QScriptEngine *engine)
      : process     (new Process     (engine))
      , application (new Application (engine))
      , eventLoop   (new EventLoop   (engine))
      , udpSocket   (new UdpSocket   (engine))
      , console     (new Console     (engine))
      , httpd       (new Httpd       (engine))
      , fileSystem  (new FileSystem  (engine))
      , fileSystemWatcher (new FileSystemWatcher (engine))
   {}
  
   QScopedPointer<Process>     process;
   QScopedPointer<Application> application;
   QScopedPointer<EventLoop>   eventLoop;
   QScopedPointer<UdpSocket>   udpSocket;
   QScopedPointer<Console>     console;
   QScopedPointer<Httpd>       httpd;
   QScopedPointer<FileSystem>  fileSystem;
   QScopedPointer<FileSystemWatcher> fileSystemWatcher;
};

Utils :: Utils (ScriptEngine const &engine, QObject *parent)
   : QObject (parent)
   , d (new Utils::Private (static_cast<QScriptEngine*>(engine.ref())))
{
}

Utils :: ~Utils()
{}

