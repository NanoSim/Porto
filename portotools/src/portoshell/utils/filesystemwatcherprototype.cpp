#include <QtCore>
#include <QScriptEngine>
#include "filesystemwatcherprototype.h"
#include "protocall.h"

FileSystemWatcherPrototype :: FileSystemWatcherPrototype (QObject *parent)
   : QObject (parent)
{
   auto fs = qscriptvalue_cast <QFileSystemWatcher*> (thisObject());
   if (fs) {
      connect (fs, SIGNAL(directoryChanged(const QString&)), SIGNAL(directoryChanged(const QString&)));
      connect (fs, SIGNAL(fileChanged(const QString&)), SIGNAL(fileChanged(const QString&)));
   }
}

FileSystemWatcherPrototype :: ~FileSystemWatcherPrototype()
{
  QTextStream(stdout) << "FileSystemWatcherPrototype :: ~FileSystemWatcherPrototype()\n";
}

QScriptValue FileSystemWatcherPrototype :: addPath(QString const &path)
{
   return protocall<QFileSystemWatcher>(thisObject(), this->parent(),
					[=](QFileSystemWatcher *fsw)
					-> bool
      {
	 return fsw->addPath(path);
      });
}

QScriptValue FileSystemWatcherPrototype :: addPaths(QStringList const &paths)
{
   return protocall_sequence<QFileSystemWatcher>(thisObject(), this->parent(),
						 [=](QFileSystemWatcher *fsw)
						 -> QStringList
      {
	 return fsw->addPaths(paths);
      });
}

QScriptValue FileSystemWatcherPrototype :: directories() const
{
   return protocall_sequence<QFileSystemWatcher>(thisObject(), this->parent(),
						 [](QFileSystemWatcher *fsw)
						 -> QStringList
      {
	 return fsw->directories();
      });
}

QScriptValue FileSystemWatcherPrototype :: files() const
{
   return protocall_sequence<QFileSystemWatcher>(thisObject(), this->parent(),
						 [](QFileSystemWatcher *fsw)
						 -> QStringList
      {
	 return fsw->files();
      });
}

QScriptValue FileSystemWatcherPrototype :: removePath(QString const &path)
{
   return protocall<QFileSystemWatcher>(thisObject(), this->parent(),
					[=](QFileSystemWatcher *fsw)
					-> bool
      {
	 return fsw->removePath(path);
      });
}

QScriptValue FileSystemWatcherPrototype :: removePaths(QStringList const &paths)
{
   return protocall_sequence<QFileSystemWatcher>(thisObject(), this->parent(),
						 [=](QFileSystemWatcher *fsw)
						 -> QStringList
      {
	 return fsw->removePaths(paths);
      });
}

