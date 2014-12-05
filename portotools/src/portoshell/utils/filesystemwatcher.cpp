#include <QScriptEngine>
#include <QScriptContext>
#include <QEventLoop>
#include <QFileSystemWatcher>
#include <QTextStream>
#include "filesystemwatcher.h"
#include "filesystemwatcherprototype.h"

Q_DECLARE_METATYPE(QFileSystemWatcher*)

class FileSystemWatcher::Private
{
   friend class FileSystemWatcher;
   Private() {
   }

   FileSystemWatcherPrototype *proto;
};

static QScriptValue createFileSystemWatcher(QScriptContext *context, QScriptEngine *engine)
{
   QFileSystemWatcher *fsw = nullptr;
   if (context->argumentCount() < 1) {
      fsw = new QFileSystemWatcher (engine);
   } else if (context->argumentCount() == 1 && context->argument(0).isArray()) {
      QStringList paths;
      qScriptValueToSequence(context->argument(0), paths);
      fsw = new QFileSystemWatcher (paths, engine);
   } else {
      context->throwError("Illegal arguments");
      return engine->undefinedValue();
   }
   Q_CHECK_PTR (fsw);
   return engine->newQObject (fsw, QScriptEngine::AutoOwnership);
}

FileSystemWatcher :: FileSystemWatcher (QObject *parent)
   : QObject (parent)
{
}

FileSystemWatcher :: FileSystemWatcher (QScriptEngine *engine)
   : QObject (engine)
   , d (new FileSystemWatcher::Private())
{
   Q_CHECK_PTR (engine);
   d->proto = q_check_ptr (new FileSystemWatcherPrototype (engine));
   engine->setDefaultPrototype (qMetaTypeId<QFileSystemWatcher*>(),
				engine->newQObject (d->proto));
   auto ctor    = engine->newFunction (createFileSystemWatcher);
   auto metaObj = engine->newQMetaObject (&QFileSystemWatcher::staticMetaObject, ctor);   
   auto global  = engine->globalObject();
   global.setProperty ("FileSystemWatcher", metaObj);
}


FileSystemWatcher :: ~FileSystemWatcher()
{
}
