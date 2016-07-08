#include <Soft>
#include <QScriptValue>
#include <QScriptEngine>

#include "utils.h"
#include "procs.h"
#include "application.h"
#include "eventloop.h"
#include "udpsocket.h"
#include "console.h"
#include "filesystem.h"
#include "filesystemwatcher.h"
#include "networkaccessmanager.h"
#include "networkreply.h"
#include "httpd.h"
#include "hostinfo.h"
#include "concurrent.h"
#include "asyncinterface.h"
#include "state.h"
#include "base64.h"

using soft::ScriptEngine;

class Utils::Private
{
   friend class Utils;
   Private(QScriptEngine *engine)
      : procs       (new Process     (engine))
      , application (new Application (engine))
      , eventLoop   (new EventLoop   (engine))
      , udpSocket   (new UdpSocket   (engine))
      , console     (new Console     (engine))
      , httpd       (new Httpd       (engine))
      , fileSystem  (new FileSystem  (engine))
      , fileSystemWatcher (new FileSystemWatcher (engine))
      , hostInfo    (new HostInfo    (engine))
      , concurrent  (new Concurrent  (engine))
      , async       (new AsyncInterface (engine))
      , base64      (new Base64      (engine))
      , state       (new State       (engine))
      , networkAccessManager (new NetworkAccessManager (engine))
      , networkReply (new NetworkReply (engine))
   {}
  
   QScopedPointer<Process>     procs;
   QScopedPointer<Application> application;
   QScopedPointer<EventLoop>   eventLoop;
   QScopedPointer<UdpSocket>   udpSocket;
   QScopedPointer<Console>     console;
   QScopedPointer<Httpd>       httpd;
   QScopedPointer<FileSystem>  fileSystem;
   QScopedPointer<FileSystemWatcher> fileSystemWatcher;
   QScopedPointer<HostInfo>    hostInfo;
   QScopedPointer<Concurrent>  concurrent;
   QScopedPointer<AsyncInterface> async;
   QScopedPointer<Base64>       base64;
   QScopedPointer<State>        state;
  QScopedPointer<NetworkAccessManager> networkAccessManager;
  QScopedPointer<NetworkReply> networkReply;
};

Utils :: Utils (ScriptEngine const &engine, QObject *parent)
   : QObject (parent)
   , d (new Utils::Private (static_cast<QScriptEngine*>(engine.ref())))
{
  //QLoggingCategory::setFilterRules("qt.network.ssl.warning=false");
}

Utils :: ~Utils()
{}

