#include <QScriptEngine>
#include <QScriptContext>
#include <QtCore>
#include "console.h"

Console :: Console (QObject * parent)
  : QObject (parent)
{}

namespace console {

  namespace {
    static QString recursiveArg(QString const &base, QStringList const &args)
    {
      if (args.count() == 0) return base;
      return recursiveArg(base.arg(args[0]), args.mid(1));
    }

    static QScriptValue formattetOutput(QScriptContext *context, QScriptEngine *engine, QTextStream &stream)
    {
      if (context->argumentCount() > 0) {
	QString const msg = context->argument(0).toString();
	QStringList rest;
	for (int i = 1; i < context->argumentCount(); ++i) {
	  rest << context->argument(i).toString();
	}
	stream << recursiveArg(msg, rest) << endl;
      }
      return engine->undefinedValue();
    }      
  }

  static QScriptValue log (QScriptContext *context, QScriptEngine *engine)
  {
    QTextStream stream(stdout);
    return formattetOutput (context, engine, stream);
  }

  static QScriptValue err (QScriptContext *context, QScriptEngine *engine)
  {
    QTextStream stream(stderr);
    return formattetOutput (context, engine, stream);
  }
  
  static QScriptValue trace (QScriptContext *context, QScriptEngine *engine)
  {
    QTextStream(stderr) << context->backtrace().join("\n") << endl;
    return engine->undefinedValue();
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

Console :: Console (QScriptEngine * engine)
  : QObject (engine)
{
  Q_CHECK_PTR (engine);
  auto globalObj = engine->globalObject();
  auto console = engine->newObject();
  globalObj.setProperty("console", console);

  registerFunction(engine, "log", console::log, &console, "Prints to stdout with newline.");
  registerFunction(engine, "info", console::log, &console, "Same as console.log.");
  registerFunction(engine, "error", console::err, &console, "Same as console.log, but writes to stderr.");
  registerFunction(engine, "warn", console::err, &console, "Same as console.log, but writes to stderr.");
  registerFunction(engine, "trace", console::trace, &console, "Print stack trace to stderr.");
}

Console :: ~Console()
{
}
