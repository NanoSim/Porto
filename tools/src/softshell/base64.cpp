#include <QtScript/QScriptEngine>
#include <QtScript/QScriptContext>
#include <QtScript/QScriptValueList>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QTextStream> // remove this
#include <QByteArray>
#include "base64.h"
#include "registerfunction.h"

namespace base64 {
  static QScriptValue encode (QScriptContext *context, QScriptEngine *engine)
  {
    if (context->argumentCount() < 1) {
      return engine->undefinedValue();
    }
    
    QScriptValue ret(context->argument(0)
		     .toString()
		     .toLocal8Bit()
		     .toBase64()
		     .constData());
    return ret;
  }

  static QScriptValue decode (QScriptContext *context, QScriptEngine *engine)
  {
    if (context->argumentCount() < 1) {
      return engine->undefinedValue();
    }

    QScriptValue ret(QByteArray::fromBase64(context->argument(0)
					    .toString()
					    .toLocal8Bit())
		     .constData());
		     
    return ret;
  }     
}

Base64 :: Base64 (QScriptEngine *engine)
  : QObject (engine)
{
  Q_CHECK_PTR (engine);
  auto globalObj = engine->globalObject();
  auto base64 = engine->newObject();
  globalObj.setProperty("base64", base64);

  registerFunction (engine, "encode", base64::encode, &base64, "encode into Base64-encoded data (RFC 4648)");
  registerFunction (engine, "decode", base64::decode, &base64, "decode Base64-encoded data");
}

Base64 :: Base64 (QObject *parent)
  : QObject (parent)
{}

Base64 :: ~Base64 ()
{}
