#include <QtScript/QScriptEngine>
#include <QtScript/QScriptContext>
#include <QtScript/QScriptValueList>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QTextStream>
#include "filesystem.h"

FileSystem :: FileSystem (QObject *parent)
   : QObject (parent)
{}

namespace filesystem {
   static QScriptValue exists (QScriptContext *context, QScriptEngine *engine)
   {
      if (context->argumentCount() < 1) return engine->undefinedValue();
      auto filename = context->argument(0).toString();
      return engine->toScriptValue (QFile::exists(filename));
   }

   static QScriptValue copy (QScriptContext *context, QScriptEngine *engine)
   {
      if (context->argumentCount() != 2) return engine->toScriptValue(false);
      auto fileName = context->argument(0).toString();
      auto newName =  context->argument(1).toString();
      return engine->toScriptValue (QFile::copy(fileName, newName));
   }

   static QScriptValue remove (QScriptContext *context, QScriptEngine *engine)
   {
      if (context->argumentCount() != 1) return engine->toScriptValue(false);
      auto fileName = context->argument(0).toString();
      return engine->toScriptValue (QFile::remove(fileName));
   }

   static QScriptValue rename (QScriptContext *context, QScriptEngine *engine)
   {
      if (context->argumentCount() != 2) return engine->toScriptValue(false);
      auto oldName = context->argument(0).toString();
      auto newName =  context->argument(1).toString();
      return engine->toScriptValue (QFile::rename(oldName, newName));
   }

   static QScriptValue currentPath(QScriptContext *context, QScriptEngine *engine)
   {
     Q_UNUSED(context);
     return engine->toScriptValue (QDir::currentPath());
   }

   static QScriptValue setCurrentPath(QScriptContext *context, QScriptEngine *engine)
   {
      if (context->argumentCount() != 1) return engine->toScriptValue(false);
      auto pathName = context->argument(0).toString();
      return engine->toScriptValue (QDir::setCurrent(pathName));
   }

   static QScriptValue readFile(QScriptContext *context, QScriptEngine *engine)
   {
      if (context->argumentCount() < 1) return engine->toScriptValue(false);
      auto fileName = context->argument(0).toString();
      auto fun = (context->argumentCount() > 1 ? 
		  context->argument(1) :
		  engine->undefinedValue());

      QFile file(fileName);
      if (!file.open (QIODevice::ReadOnly | QIODevice::Text)) 
      {
	 auto errorString = engine->toScriptValue(file.errorString());
	 if (fun.isFunction()) {
	    QScriptValueList list = QScriptValueList() << errorString << QScriptValue();
	    fun.call(context->thisObject(), list);
	 }
	 return engine->toScriptValue(false);
      }
      
      if (fun.isFunction()) {
	 QTextStream in(&file);
	 QScriptValueList list = QScriptValueList() << QScriptValue(false) << engine->toScriptValue(in.readAll());
	 fun.call (context->thisObject(), list);
      }

      return engine->toScriptValue(true);
   }
     
  static QScriptValue writeFile(QScriptContext *context, QScriptEngine *engine)
  {
    if (context->argumentCount() < 2) return engine->toScriptValue(false);
    auto fileName = context->argument(0).toString();
    auto data = context->argument(1).toString();
    auto fun = (context->argumentCount() > 2 ? 
		context->argument(2) :
		engine->undefinedValue());
    
    QFile file(fileName);
    if (!file.open (QIODevice::WriteOnly | QIODevice::Text)) {
      auto errorString = engine->toScriptValue (file.errorString());
      if (fun.isFunction()) {
	QScriptValueList list = QScriptValueList() << errorString << QScriptValue();
	fun.call(context->thisObject(), list);	
      }
      return engine->toScriptValue(false);
    }

    QTextStream out(&file);
    out << data;

    return engine->toScriptValue(true);
  }
  
  static QScriptValue baseName(QScriptContext *context, QScriptEngine *engine)
  {
    if (context->argumentCount() < 1)
      return engine->undefinedValue();

    auto filename = context->argument(0).toString();
    QFileInfo info(filename);
    auto baseName = info.baseName();
    return engine->toScriptValue(baseName);
  }

  
} // end of namespace

template <class Function>
static void registerFunction (QScriptEngine *engine, 
			      QString const & name, 
			      Function fn, 
			      QScriptValue *obj,
			      QString const & doc = QString())
{
   auto scriptFunction = engine->newFunction(fn);
   if (!doc.isEmpty()) {
      scriptFunction.setProperty("^doc", doc);
   }
   obj->setProperty(name, scriptFunction);
}

FileSystem :: FileSystem (QScriptEngine *engine)
   : QObject (engine)
{   
   Q_CHECK_PTR (engine);
   auto globalObj = engine->globalObject();
   auto fs = engine->newObject();
   globalObj.setProperty("fs", fs);

   registerFunction (engine, "exists",         filesystem::exists,         &fs, "Returns true if the file specified by fileName exists; otherwise returns false.");
   registerFunction (engine, "copy",           filesystem::copy,           &fs, "Copies the file fileName to newName. Returns true if successful; otherwise returns false.");
   registerFunction (engine, "remove",         filesystem::remove,         &fs, "Removes the file specified by fileName(). Returns true if successful; otherwise returns false.");
   registerFunction (engine, "rename",         filesystem::rename,         &fs, "Renames the file oldName to newName. Returns true if successful; otherwise returns false.");
   registerFunction (engine, "currentPath",    filesystem::currentPath,    &fs, "Returns the absolute path of the application's current directory.");
   registerFunction (engine, "setCurrentPath", filesystem::setCurrentPath, &fs, "Sets the application's current working directory. Returns true if the directory was successfully changed; otherwise returns false.");    
   registerFunction (engine, "readFile",       filesystem::readFile,       &fs, "Asynchronously reads the entire contents of a file.");
   registerFunction (engine, "writeFile",      filesystem::writeFile,      &fs, "Write data to a file, creating the file if it does not yet exists.");
   registerFunction (engine, "baseName",       filesystem::baseName,       &fs, "Returns the base name of the file.");
}

FileSystem :: ~FileSystem()
{}
