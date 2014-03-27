#include <QtScript>
#include <QtCore/QCoreApplication>
#include <QtConcurrent>
#include <QtConcurrentRun>
#include <QFutureWatcher>
#include <QFuture>
#include <QThreadPool>
#include <QtCore>

#include "sys.h"

QFutureSynchronizer<QScriptValue> futureSynchronizer;

QString read(const QString &filename)
{
  QFile file(filename);
  if (!file.open (QIODevice::ReadOnly | QIODevice::Text))
    return QString();
  
  QTextStream in(&file);
  return in.readAll();
}

bool reportError( QScriptEngine *engine, const QScriptValue &retval)
{
  if( engine->hasUncaughtException() ) {
    QStringList const backtrace = engine->uncaughtExceptionBacktrace();
    QTextStream(stderr) << qPrintable(retval.toString()) 
                        << "\n\t== STACKTRACE ==\n\t" << qPrintable(backtrace.join("\n\t"))
                        << "\n";
    
    engine->clearExceptions();
    
    return true;    
  }  
  return false;  
}

QScriptValue exceptionBacktrace(QScriptContext *, QScriptEngine *engine)
{
  if( engine->hasUncaughtException() ) {
    QStringList const bracktrace = engine->uncaughtExceptionBacktrace();
    QTextStream(stderr) << "\t" << qPrintable(bracktrace.join("\n\t"));
    engine->clearExceptions();
  }
  return engine->undefinedValue();
}

QScriptValue syncInfo(QScriptContext *, QScriptEngine *)
{
  QMap<QString, int> statusMap;
  foreach( QFuture<QScriptValue> const &future, futureSynchronizer.futures() ) {
    if( future.isRunning() ) statusMap["running"]++;
    if( future.isCanceled() ) statusMap["canceled"]++;
    if( future.isFinished() ) statusMap["finished"]++;
    if( future.isPaused() ) statusMap["paused"]++;
    if( future.isStarted() ) statusMap["started"]++;
  }
  QTextStream(stdout) << "Max threads:   " << QThreadPool::globalInstance()->maxThreadCount() << endl;
  QTextStream(stdout) << "Running jobs:  " << statusMap["running"] << endl;
  QTextStream(stdout) << "Canceled jobs: " << statusMap["canceled"] << endl;
  QTextStream(stdout) << "Finished jobs: " << statusMap["finished"] << endl;
  QTextStream(stdout) << "Paused jobs:   " << statusMap["paused"] << endl;
  QTextStream(stdout) << "Started jobs:  " << statusMap["started"] << endl;
  return QScriptValue();
}

QScriptValue sysSetMaxThreadCount(QScriptContext *context, QScriptEngine *engine)
{
  Q_UNUSED(engine);

  if(context->argumentCount() < 1)
    context->throwError(QString("Invalid argument"));
  
  if(!context->argument(0).isNumber() )
    context->throwError(QString("The Argument needs to be an integer"));
  
  auto const newMaxThreadCount = context->argument(0).toInt32();
  auto const oldMaxThreadCount = QThreadPool::globalInstance()->maxThreadCount();

  QThreadPool::globalInstance()->setMaxThreadCount(newMaxThreadCount);
  return QScriptValue(oldMaxThreadCount);
}

QScriptValue sysExecuteAndWait(QScriptContext *context, QScriptEngine *)
{
  if(context->argumentCount() < 1)
    context->throwError(QString("Invalid argument"));
  
  QString const command = context->argument(0).toString();
  QStringList argList;
  for( int i = 1; i < context->argumentCount(); ++i)
    argList << context->argument(i).toString();

  QProcess proc;
  proc.start(command, argList);
  proc.waitForFinished(-1);
  QScriptValue const ret (proc.readAllStandardOutput().constData());

  return ret;
}

QScriptValue sysExec(QScriptContext *context, QScriptEngine *engine)
{
  QString const command = context->argument(0).toString();
  QScriptValue callback = context->argument(1);

  // .. perform something

  Sys sys(callback);
  futureSynchronizer.addFuture( QtConcurrent::run(sys, &Sys::exec, command));

  return engine->undefinedValue();
}

QScriptValue sysClearFutures(QScriptContext *context, QScriptEngine *engine)
{
  Q_UNUSED(context);
  futureSynchronizer.clearFutures();
  return engine->undefinedValue();
}

QScriptValue sysWaitForFinished(QScriptContext *context, QScriptEngine *engine)
{
  Q_UNUSED(context);
  futureSynchronizer.waitForFinished();
  return engine->undefinedValue();
}

QScriptValue externalStorageAddExternalStorageDriver(QScriptContext *context, QScriptEngine *engine)
{
  qWarning() << "Unimplemented!";
  if(context->argumentCount() < 1)
    context->throwError(QString("Invalid argument"));
  
  auto const driverName = context->argument(0).toString();
  return engine->undefinedValue();  
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
      qDebug() << "loading" << file;
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
  if( reportError(engine, ret) )
    return engine->undefinedValue();

  return ret;
}

QScriptValue importExtension(QScriptContext *context, QScriptEngine *engine)
{
  if(context->argumentCount() != 1 ) 
    return context->throwError("checkSyntax() takes exactly one argument");

  return engine->importExtension(context->argument(0).toString());
}

QScriptValue isIncompleteSyntax(QScriptContext *context, QScriptEngine *engine)
{
  Q_UNUSED(engine);
  if(context->argumentCount() != 1 ) 
    return context->throwError("fileRead() takes exactly one argument");
  QString const source = context->argument(0).toString();
  QScriptSyntaxCheckResult res = QScriptEngine::checkSyntax(source);
  return QScriptValue(res.state() == QScriptSyntaxCheckResult::Intermediate);
}

QScriptValue isErrorSyntax(QScriptContext *context, QScriptEngine *engine)
{
  Q_UNUSED(engine);
  if(context->argumentCount() != 1 ) 
    return context->throwError("fileRead() takes exactly one argument");
  QString const source = context->argument(0).toString();
  QScriptSyntaxCheckResult res = QScriptEngine::checkSyntax(source);
  return QScriptValue(res.state() == QScriptSyntaxCheckResult::Error);
}

QScriptValue errorMessage(QScriptContext *context, QScriptEngine *engine)
{
  if(context->argumentCount() != 1 ) 
    return context->throwError("fileRead() takes exactly one argument");
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
    return context->throwError("fileRead() takes exactly one argument");
  QString const source = context->argument(0).toString();
  QScriptSyntaxCheckResult res = QScriptEngine::checkSyntax(source);
  return QScriptValue(res.state() == QScriptSyntaxCheckResult::Valid);
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
      QScriptValue ret = engine->evaluate(in.readAll(), filename);
      
      if( reportError(engine, ret) )
	return engine->undefinedValue();
    
      return ret;    
    }
  return context->throwError("Could not open " + filename + " for reading.");
}

int main(int argc, char **argv)
{  
  Q_INIT_RESOURCE(porto);
  QCoreApplication app(argc,argv);

  QProcessEnvironment const &env = QProcessEnvironment::systemEnvironment();
  if( env.contains("PORTO_MODULES") ) {
#ifdef WIN32
    QChar const separator(';');
#else
    QChar const separator(':');
#endif
    foreach( QString const & p, env.value("PORTO_MODULES").split(separator) ) {
      app.addLibraryPath(p);
    }
  }
  // qDebug() << qPrintable(QCoreApplication::libraryPaths().join("\n"));
    
  /*
   * Base
   */
  QScriptEngine engine;  
  engine.globalObject().setProperty("__global__", engine.globalObject());
  QScriptValue readlineFunction = engine.newFunction(readline);
  engine.globalObject().setProperty("readline", readlineFunction);
  QScriptValue writelineFunction = engine.newFunction(writeline);
  engine.globalObject().setProperty("writeline", writelineFunction);
  QScriptValue loadFunction = engine.newFunction(load);
  engine.globalObject().setProperty("load", loadFunction);
  engine.globalObject().setProperty("include", loadFunction);
  QScriptValue checkSyntaxFunction  = engine.newFunction(checkSyntax);
  engine.globalObject().setProperty("checkSyntax", checkSyntaxFunction);
  QScriptValue fileReadFunction  = engine.newFunction(fileRead);
  engine.globalObject().setProperty("readFile", fileReadFunction);
  QScriptValue importExtensionFunction  = engine.newFunction(importExtension);
  engine.globalObject().setProperty("importExtension", importExtensionFunction);
  QScriptValue exceptionBacktraceFunction = engine.newFunction(exceptionBacktrace);
  engine.globalObject().setProperty("exceptionBacktrace", exceptionBacktraceFunction);
 
  QScriptValue uuidgenFunction = engine.newFunction(uuidgen);
  engine.globalObject().setProperty("uuidgen", uuidgenFunction);
  QScriptValue requireFunction = engine.newFunction(require);
  engine.globalObject().setProperty("require", requireFunction);
  QScriptValue writeFunction = engine.newFunction(writeFile);
  writeFunction.setProperty("^doc", "Syntax: writeFile( path, buffer )");
  engine.globalObject().setProperty("writeFile", writeFunction);

  QScriptValue isValidSyntaxFunction = engine.newFunction(isValidSyntax);
  engine.globalObject().setProperty("isValidSyntax", isValidSyntaxFunction);
  QScriptValue isErrorSyntaxFunction = engine.newFunction(isErrorSyntax);
  engine.globalObject().setProperty("isErrorSyntax", isErrorSyntaxFunction);
  QScriptValue isIncompleteSyntaxFunction = engine.newFunction(isIncompleteSyntax);
  engine.globalObject().setProperty("isIncompleteSyntax", isIncompleteSyntaxFunction);
  QScriptValue errorMessageFunction = engine.newFunction(errorMessage);
  engine.globalObject().setProperty("errorMessage", errorMessageFunction);

  /*
   * Sys
   */
  QScriptValue sysObject = engine.newObject();
  engine.globalObject().setProperty("sys", sysObject);
  QScriptValue sysExecFunction = engine.newFunction(sysExec);
  sysObject.setProperty("exec", sysExecFunction);
  QScriptValue sysWaitForFinishedFunction = engine.newFunction(sysWaitForFinished);
  sysObject.setProperty("waitForFinished", sysWaitForFinishedFunction);
  QScriptValue sysClearFuturesFunction = engine.newFunction(sysClearFutures);
  sysObject.setProperty("clearFutures", sysClearFuturesFunction);

  QScriptValue sysExecuteAndWaitFunction = engine.newFunction(sysExecuteAndWait);
  sysExecuteAndWaitFunction.setProperty("^doc", "This function executes a shell command and returns output from stdout.");
  sysObject.setProperty("executeAndWait", sysExecuteAndWaitFunction);

  QScriptValue sysSetMaxThreadCountFunction = engine.newFunction(sysSetMaxThreadCount);
  sysSetMaxThreadCountFunction.setProperty("^doc", "Set the maximum number of threads used by the thread pool.");
  sysObject.setProperty("setMaxThreadCount", sysSetMaxThreadCountFunction);

  QScriptValue sysSyncInfo = engine.newFunction(syncInfo);
  sysObject.setProperty("info", sysSyncInfo);

  /*
   * External Storage
   */
  QScriptValue externalStorageObject = engine.newObject();
  engine.globalObject().setProperty("ExternalStorage", externalStorageObject);
  QScriptValue externalStorageAddExternalStorageDriverFunction = engine.newFunction(externalStorageAddExternalStorageDriver);
  externalStorageObject.setProperty("addExternalStorageDriver", externalStorageAddExternalStorageDriverFunction);
  
  /* 
   *
   */
  QScriptProgram program(read(":/res/repl.js"), "repl.js");
  QScriptValue result = engine.evaluate(program);
  if (reportError(&engine, result)) return 1;
  
  QScriptValue mainFunction = engine.globalObject().property("__main__");

  QScriptValue args = engine.toScriptValue<QStringList>(QCoreApplication::arguments());
  QScriptValue mainReturn = mainFunction.call( QScriptValue(), QScriptValueList() << args );  

  if (reportError(&engine, mainReturn)) return 1;
  return mainReturn.toInteger();
}
