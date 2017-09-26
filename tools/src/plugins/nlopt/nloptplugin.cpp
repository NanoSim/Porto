#include <cmath>

#include <QScriptEngine>
#include <QScriptContext>
#include <QScriptValue>

#include <nlopt.h>

#include "nloptplugin.h"

#include "qnlopt.h"
#include "qnloptprototype.h"

#include "register.hh"

using soft::nlopt::QNLopt;
Q_DECLARE_METATYPE(QNLopt*)

NLoptPlugin :: ~NLoptPlugin()
{
}

static QScriptValue createNLopt(QScriptContext *ctx, QScriptEngine *engine)
{
  QNLopt *opt = nullptr;
  if (ctx->argumentCount() >= 2 && ctx->argument(0).isNumber() && ctx->argument(1).isNumber()) {
    auto algorithm = ctx->argument(0).toInt32();
    auto dims      = ctx->argument(1).toUInt32();
    opt = new QNLopt(algorithm, dims, engine);
  } else {
    ctx->throwError (QScriptContext::SyntaxError, "Illegal arguments");
    return engine->undefinedValue();
  }
  return engine->newQObject (opt, QScriptEngine::ScriptOwnership);
}

template <class T>
inline void registerClass(QScriptValue &parentObj, QString const &name, QObject *that)
{
   auto module = new T(that);
   auto qobj = parentObj.engine()->newQObject(module, QScriptEngine::ScriptOwnership);
   parentObj.setProperty(name, qobj);
}

void NLoptPlugin :: registerPlugin(QScriptEngine *engine)
{
  Q_CHECK_PTR (engine);
  auto globalObj = engine->globalObject();
  globalObj.setProperty("NLOPT_LD_MMA", NLOPT_LD_MMA);
  globalObj.setProperty("HUGE_VAL", HUGE_VAL);

  registerPrototype<QNLoptPrototype, QNLopt> (engine);
  registerConstructor<QNLopt>(engine, globalObj, "NLopt", createNLopt);
}
