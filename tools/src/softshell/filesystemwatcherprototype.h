#ifndef FILESYSTEMWATCHERPROTOTYPE_H_DEF
#define FILESYSTEMWATCHERPROTOTYPE_H_DEF

#include <QScriptable>
#include <QObject>
#include <QScriptValue>
#include <QString>
class FileSystemWatcherPrototype : public QObject,
				   public QScriptable
{
   Q_OBJECT
public:
   explicit FileSystemWatcherPrototype(QObject *parent = nullptr);
   virtual ~FileSystemWatcherPrototype();

public slots:
   QScriptValue addPath(QString const &path);
   QScriptValue addPaths(QStringList const &paths);
   QScriptValue directories() const;
   QScriptValue files() const;
   QScriptValue removePath(QString const &path);
   QScriptValue removePaths(QStringList const &path);
   
signals:
   void directoryChanged(QString const &);
   void fileChanged(QString const &);
};
#endif // FILESYSTEMWATCHERPROTOTYPE_H_DEF
