#ifndef FILESYSTEMWATCHER_H_DEF
#define FILESYSTEMWATCHER_H_DEF

#include <QObject>
#include <QScopedPointer>

class QScriptEngine;
class FileSystemWatcher : public QObject
{
   Q_OBJECT

public:
   explicit FileSystemWatcher (QScriptEngine *object);
   virtual ~FileSystemWatcher();

private:
   class Private;
   QScopedPointer<Private> d;

   FileSystemWatcher (QObject *parent = nullptr);
};

#endif // FILESYSTEMWATCHER_H_DEF
