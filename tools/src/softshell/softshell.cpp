#include <Soft>
#include <QtGlobal>
#include <QtScript>
#include <QtCore>

#include "soft.h"
#include "softns.h"

/*!
  \file softshell.cpp
  \ingroup soft-tools

  This file defines commands for softshell
 */

ANONYMOUS_BEGIN_NAMESPACE

QString read(const QString &filename)
{
   QFile file(filename);
   if (!file.open (QIODevice::ReadOnly | QIODevice::Text))
      return QString();

   QTextStream in(&file);
   return in.readAll();
}

bool reportError( QScriptEngine *engine, const QScriptValue &retval, const QString &programFile)
{
   if( engine->hasUncaughtException() ) {
      QStringList const backtrace = engine->uncaughtExceptionBacktrace();
      QTextStream(stderr) << qPrintable(retval.toString())
                          << "\n\t== STACKTRACE ==\n\t" << qPrintable(backtrace.join("\n\t"))
                          << "\n"
                          << "In: " << programFile
                          << "\n"
                          << "Application arguments: " << QCoreApplication::arguments().join(" ")
                          << "\n";


//      engine->clearExceptions();

      return true;
   }
   return false;
}

QScriptValue exceptionBacktrace(QScriptContext *, QScriptEngine *engine)
{
   if( engine->hasUncaughtException() ) {
      QStringList const bracktrace = engine->uncaughtExceptionBacktrace();
      QTextStream(stderr) << "\t" << qPrintable(bracktrace.join("\n\t"));
//      engine->clearExceptions();
   }
   return engine->undefinedValue();
}

QScriptValue version(QScriptContext *context, QScriptEngine *engine)
{
  Q_UNUSED(context);
  QString versionString = QString("%1.%2.%3")
    .arg(QString::number(SOFT_MAJOR_VERSION))
    .arg(QString::number(SOFT_MINOR_VERSION))
    .arg(QString::number(SOFT_PATCH_VERSION));
  return engine->toScriptValue(versionString);
}

QScriptValue writeline(QScriptContext *context, QScriptEngine *engine)
{
   if(context->argumentCount() < 1) return engine->undefinedValue();
   QTextStream(stdout) << context->argument(0).toString().toUtf8();
   return engine->undefinedValue();

}

QScriptValue writeFile(QScriptContext *context, QScriptEngine *engine)
{
   if( context->argumentCount() != 2) return engine->undefinedValue();
   const QString path = context->argument(0).toString();
   const QString buffer = context->argument(1).toString();

   QFile file(path);
   if( file.open(QIODevice::WriteOnly | QIODevice::Text) ) {
      file.write(qPrintable(buffer));
   }
   return engine->undefinedValue();
}

QScriptValue readline(QScriptContext *context, QScriptEngine *engine)
{
   Q_UNUSED(engine);
   Q_UNUSED(context);

   QString buffer;
   QTextStream stream(stdin);
   buffer = stream.readLine();
   return QScriptValue(buffer);
}


QString findModule(const QString &module)
{
   // If the module is a legal filename, return the module name.
   if( module.contains(QRegExp(".*\\.js$"))) {
      if( QFile::exists(module) ) return module;
   }

   // Search through the libraries to see if the module is found.  The
   // namespace separators are replaced with '/' and the postfix '.js'
   // is added.
   foreach( const QString &path, QCoreApplication::libraryPaths() ) {
      QString copyStr(module);
      QString const file = path + "/" + copyStr.replace(QChar('.'), QChar('/')) + ".js";

      if( QFile::exists(file) ) {
   return file;
      }
   }
   return QString();
}

QScriptValue uuidgen(QScriptContext *context, QScriptEngine *engine)
{
   Q_UNUSED(context);
   Q_UNUSED(engine);

   QScriptValue const ret(QUuid::createUuid().toString());
   return ret;
}

QScriptValue isValidModule(QScriptContext *context, QScriptEngine *engine)
{
   if(context->argumentCount() != 1) return QScriptValue(false);
   const QString moduleName = context->argument(0).toString();
   return QScriptValue (!findModule(moduleName).isEmpty());
}

QScriptValue require(QScriptContext *context, QScriptEngine *engine)
{
   if(context->argumentCount() != 1) return engine->undefinedValue();
   const QString nsFile = context->argument(0).toString();
   const QString filename = findModule(nsFile);
   if( filename.isEmpty() ) {
      return context->throwError(QString("failed to find module %1").arg(nsFile));
   }

   QByteArray ba;
   {
      QTextStream stream(&ba);
      stream << "(function(exports){";
      stream << read(filename);
      stream << ";return exports;})({filename:\'" << filename << "\'});";
   }

   QScriptContext *parentContext = context->parentContext();
   context->setActivationObject( parentContext->activationObject() );
   context->setThisObject( parentContext->thisObject() );
   QScriptProgram extension(ba, filename, 1);
   const QScriptValue ret =  engine->evaluate(extension);
   if( reportError(engine, ret, filename) )
      return engine->undefinedValue();

   return ret;
}

QScriptValue importExtension(QScriptContext *context, QScriptEngine *engine)
{
   if(context->argumentCount() != 1 )
      return context->throwError("importExtension() takes exactly one argument");

   return engine->importExtension(context->argument(0).toString());
}

QScriptValue isIncompleteSyntax(QScriptContext *context, QScriptEngine *engine)
{
   Q_UNUSED(engine);
   if(context->argumentCount() != 1 )
      return context->throwError("isIncompleteSyntax() takes exactly one argument");
   QString const source = context->argument(0).toString();
   QScriptSyntaxCheckResult res = QScriptEngine::checkSyntax(source);
   return QScriptValue(res.state() == QScriptSyntaxCheckResult::Intermediate);
}

QScriptValue isErrorSyntax(QScriptContext *context, QScriptEngine *engine)
{
   Q_UNUSED(engine);
   if(context->argumentCount() != 1 )
      return context->throwError("isErrorSyntax() takes exactly one argument");
   QString const source = context->argument(0).toString();
   QScriptSyntaxCheckResult res = QScriptEngine::checkSyntax(source);
   return QScriptValue(res.state() == QScriptSyntaxCheckResult::Error);
}

QScriptValue errorMessage(QScriptContext *context, QScriptEngine *engine)
{
   if(context->argumentCount() != 1 )
      return context->throwError("errorMessage() takes exactly one argument");
   QString const source = context->argument(0).toString();
   QScriptSyntaxCheckResult res = QScriptEngine::checkSyntax(source);
   if ( res.state() == QScriptSyntaxCheckResult::Error ) {
      return QScriptValue(QString("%1 in line: %2 col: %3")
        .arg(res.errorMessage())
        .arg(res.errorLineNumber())
        .arg(res.errorColumnNumber()));
   }
   return engine->undefinedValue();
}

QScriptValue isValidSyntax(QScriptContext *context, QScriptEngine *engine)
{
   Q_UNUSED(engine);
   if(context->argumentCount() != 1 )
      return context->throwError("isValidSyntax() takes exactly one argument");
   QString const source = context->argument(0).toString();
   QScriptSyntaxCheckResult res = QScriptEngine::checkSyntax(source);
   return QScriptValue(res.state() == QScriptSyntaxCheckResult::Valid);
}

#if 0
QScriptValue objectToString(QScriptContext *context, QScriptEngine *engine)
{
   if(context->argumentCount() != 1 )
     return context->throwError("objectToString() takes exactly one argument");

   auto value = context->argument(0);
   if (value.isObject() && !value.isQObject()) {
     auto newContext = engine->pushContext();
     newContext->activationObject().setProperty("o", value);
     auto retval = engine->evaluate("JSON.stringify(o)");
     engine->popContext();
     return retval;
   }
   return engine->undefinedValue();
}
#endif


QScriptValue isArray(QScriptContext *context, QScriptEngine *engine)
{
   if(context->argumentCount() != 1 )
      return context->throwError("isArray() takes exactly one argument");
   return engine->toScriptValue(context->argument(0).isArray());
}


QScriptValue isBool(QScriptContext *context, QScriptEngine *engine)
{
   if(context->argumentCount() != 1 )
      return context->throwError("isBool() takes exactly one argument");
   return engine->toScriptValue(context->argument(0).isBool());
}


QScriptValue isDate(QScriptContext *context, QScriptEngine *engine)
{
   if(context->argumentCount() != 1 )
      return context->throwError("isDate() takes exactly one argument");
   return engine->toScriptValue(context->argument(0).isDate());
}


QScriptValue isError(QScriptContext *context, QScriptEngine *engine)
{
   if(context->argumentCount() != 1 )
      return context->throwError("isError() takes exactly one argument");
   return engine->toScriptValue(context->argument(0).isError());
}


QScriptValue isFunction(QScriptContext *context, QScriptEngine *engine)
{
   if(context->argumentCount() != 1 )
      return context->throwError("isFunction() takes exactly one argument");
   return engine->toScriptValue(context->argument(0).isFunction());
}


QScriptValue isNull(QScriptContext *context, QScriptEngine *engine)
{
   if(context->argumentCount() != 1 )
      return context->throwError("isNull() takes exactly one argument");
   return engine->toScriptValue(context->argument(0).isNull());
}


QScriptValue isNumber(QScriptContext *context, QScriptEngine *engine)
{
   if(context->argumentCount() != 1 )
      return context->throwError("isNumber() takes exactly one argument");
   return engine->toScriptValue(context->argument(0).isNumber());
}


QScriptValue isString(QScriptContext *context, QScriptEngine *engine)
{
   if(context->argumentCount() != 1 )
      return context->throwError("isString() takes exactly one argument");
   return engine->toScriptValue(context->argument(0).isString());
}

QScriptValue isRegExp(QScriptContext *context, QScriptEngine *engine)
{
   if(context->argumentCount() != 1 )
      return context->throwError("isRegExp() takes exactly one argument");
   return engine->toScriptValue(context->argument(0).isRegExp());
}

QScriptValue isUndefined(QScriptContext *context, QScriptEngine *engine)
{
   if(context->argumentCount() != 1 )
      return context->throwError("isUndefined() takes exactly one argument");
   return engine->toScriptValue(context->argument(0).isUndefined());
}

QScriptValue isObject(QScriptContext *context, QScriptEngine *engine)
{
   if(context->argumentCount() != 1 )
      return context->throwError("isObject() takes exactly one argument");
   return engine->toScriptValue(context->argument(0).isObject());
}


QScriptValue isQObject(QScriptContext *context, QScriptEngine *engine)
{
   if(context->argumentCount() != 1 )
      return context->throwError("isQObject() takes exactly one argument");
   return engine->toScriptValue(context->argument(0).isQObject());
}


QScriptValue isQMetaObject(QScriptContext *context, QScriptEngine *engine)
{
   if(context->argumentCount() != 1 )
      return context->throwError("isQMetaObject () takes exactly one argument");
   return engine->toScriptValue(context->argument(0).isQMetaObject());
}

QScriptValue fileRead(QScriptContext *context, QScriptEngine *)
{
   if(context->argumentCount() != 1 )
      return context->throwError("fileRead() takes exactly one argument");

   QString const filename = context->argument(0).toString();
   QFile file(filename);
   if( !file.open(QIODevice::Text | QIODevice::ReadOnly) )
   {
      return context->throwError("Could not open " + filename + " for reading.");
   }

   QTextStream in(&file);
   QScriptValue const retval(in.readAll());
   return retval;

   //  return engine->toScriptValue(in.readAll());
}


QScriptValue checkSyntax(QScriptContext *context, QScriptEngine *engine)
{
   if(context->argumentCount() != 1 )
      return context->throwError("checkSyntax() takes exactly one argument");

   QString const filename = context->argument(0).toString();
   QFile file(filename);
   if( !file.open(QIODevice::Text | QIODevice::ReadOnly) )
   {
      return context->throwError("Could not open " + filename + " for reading.");
   }

   QTextStream in(&file);
   QScriptSyntaxCheckResult results = QScriptEngine::checkSyntax(in.readAll());
   if( results.state() != QScriptSyntaxCheckResult::Valid )
   {
      QString errorMessage = QString("%1:%2:%3:%4")
   .arg(filename)
   .arg(QString::number(results.errorLineNumber()))
   .arg(QString::number(results.errorColumnNumber()))
   .arg(results.errorMessage());
      return context->throwError(errorMessage);
   }
   return engine->undefinedValue();
}

QScriptValue load(QScriptContext *context, QScriptEngine *engine)
{
   if(context->argumentCount() != 1 )
      return context->throwError("load() takes exactly one argument");

   QString filename = context->argument(0).toString();
   QFile file(filename);
   if( file.open(QIODevice::Text | QIODevice::ReadOnly) )
   {
      QTextStream in(&file);
      context->setActivationObject(context->parentContext()->activationObject());
      context->setThisObject(context->parentContext()->thisObject());
      auto header = in.readLine();
      // If the header is a bash-script, ignore the line
      auto ignore = (header.contains (QRegExp("^#\\!\\/.*$")));
      QScriptValue ret = engine->evaluate((ignore ? QString() : header) + in.readAll() , filename);

      if( reportError(engine, ret, filename) )
   return engine->undefinedValue();

      return ret;
   }
   return context->throwError("Could not open " + filename + " for reading.");
}

QScriptValue clearExceptions (QScriptContext *ctx, QScriptEngine *engine)
{
  Q_UNUSED(ctx);
  engine->clearExceptions();
  return engine->undefinedValue();
}

QScriptValue collectGarbage (QScriptContext *ctx, QScriptEngine *engine)
{
  Q_UNUSED(ctx);
  engine->collectGarbage();
  return engine->undefinedValue();
}

QScriptValue reportAdditionalMemoryCost (QScriptContext *ctx, QScriptEngine *engine)
{
  if (ctx->argumentCount() != 1)
    return ctx->throwError("reportAdditionalMemoryCost(int) missing argument");

  auto size = ctx->argument(0).toInt32();
  engine->reportAdditionalMemoryCost(size);
  return engine->undefinedValue();
}

void loadSoftModule()
{
   QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath() + "/modules");
   QProcessEnvironment const &env = QProcessEnvironment::systemEnvironment();
   if( env.contains("SOFT_MODULES") ) {
#ifdef WIN32
      QChar const separator(';');
#else
      QChar const separator(':');
#endif
      foreach( QString const & p, env.value("SOFT_MODULES").split(separator) ) {
   qApp->addLibraryPath(p);
      }
   }

}

template <class Function>
static void registerFunction (QScriptEngine *engine,
            QString const & name,
            Function fn,
            QString const & doc = QString())
{
   auto scriptFunction = engine->newFunction(fn);
   if (!doc.isEmpty()) {
      scriptFunction.setProperty("^doc", doc);
   }
   engine->globalObject().setProperty(name, scriptFunction);
}

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

void registerBase(QScriptEngine *engine)
{
   engine->globalObject().setProperty("__global__", engine->globalObject());

   registerFunction(engine, "version", version);
   registerFunction(engine, "readline", readline);
   registerFunction(engine, "writeline", writeline);
   registerFunction(engine, "load", load);
   registerFunction(engine, "include", load);
   registerFunction(engine, "checkSyntax", checkSyntax);
   registerFunction(engine, "readFile", fileRead);
   registerFunction(engine, "importExtension", importExtension);
   registerFunction(engine, "exceptionBacktrace", exceptionBacktrace);
   registerFunction(engine, "uuidgen", uuidgen);
   registerFunction(engine, "require", require);
   registerFunction(engine, "isValidModule", isValidModule, "Check if a module is present");
   registerFunction(engine, "writeFile", writeFile, "Syntax: writeFile( path, buffer )");
   registerFunction(engine, "isValidSyntax", isValidSyntax);
   registerFunction(engine, "isErrorSyntax", isErrorSyntax);
   registerFunction(engine, "isIncompleteSyntax", isIncompleteSyntax);
   registerFunction(engine, "errorMessage", errorMessage);
   registerFunction(engine, "clearExceptions", clearExceptions, "Clears any uncaught exceptions");
   registerFunction(engine, "collectGarbage", collectGarbage, "Runs the garbage collector");
   registerFunction(engine, "reportAdditionalMemoryCost", reportAdditionalMemoryCost, "Reports and additional memory cost of the given size, measured in bytes, to the garbage collector");

   registerFunction(engine, "isArray", isArray);
   registerFunction(engine, "isBool", isBool);
   registerFunction(engine, "isDate", isDate);
   registerFunction(engine, "isError", isError);
   registerFunction(engine, "isFunction", isFunction);
   registerFunction(engine, "isNull", isNull);
   registerFunction(engine, "isNumber", isNumber);
   registerFunction(engine, "isString", isString);
   registerFunction(engine, "isRegExp", isRegExp);
   registerFunction(engine, "isUndefined", isUndefined);
   registerFunction(engine, "isObject", isObject);
   registerFunction(engine, "isQObject", isQObject);
   registerFunction(engine, "isQMetaObject", isQMetaObject);

}

ANONYMOUS_END_NAMESPACE

int startRepl (soft::ScriptEngine const &e)
{
  auto engine = static_cast<QScriptEngine*>(e.ref());

  loadSoftModule();
  registerBase(engine);

  QString programFile = (QCoreApplication::arguments().count() < 2 ?
                           QString(":/resources/repl.js") :
                           QCoreApplication::arguments()[1]);

  QFile file(programFile);
  if (!file.open (QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream(stderr) << file.errorString() << endl;
    return 1;
  }
  //   QFileInfo info(programFile);
  //   QDir::setCurrent(info.path());

  QTextStream in(&file);
  QString buffer;
  auto header = in.readLine();
  if (!header.contains(QRegExp("^[#!/]{1}.*$"))) {
    buffer = header;
  }

  buffer += in.readAll();
  QScriptProgram program(buffer, programFile);
  auto result = engine->evaluate(program);
  if (reportError(engine, result, program.fileName())) {
    return 1;
  }

  auto mainFunction = engine->globalObject().property("__main__");
  auto argList = qScriptValueFromSequence(engine, QCoreApplication::arguments().mid(1));
  auto args = QScriptValueList() << argList;
  auto mainReturn = mainFunction.call (QScriptValue(), args);

  return mainReturn.toInteger();
}
