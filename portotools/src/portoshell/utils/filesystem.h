#ifndef FILESYSTEM_H_DEF
#define FILESYSTEM_H_DEF

#include <QObject>

class QScriptEngine;
class FileSystem : public QObject
{
   Q_OBJECT
public:
   explicit FileSystem (QScriptEngine *engine);
   virtual ~FileSystem();

private:
   FileSystem (QObject *parent = nullptr);
};

#endif // FILESYSTEM_H_DEF
