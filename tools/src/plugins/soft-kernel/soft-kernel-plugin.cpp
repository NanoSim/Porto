#include <QtCore>
#include <QtScript>
#include "qgenericentity.h"
#include "qcollection.h"
#include "qstorage.h"
#include "register.hh"
#include "soft-kernel-plugin.h"

// Not used, but still cool. allows for custom prototyping from JavaScript on registered types
#define Q_SCRIPT_DECLARE_QMETAOBJECT_MY(T, _Arg1) \
  template<> inline QScriptValue qscriptQMetaObjectConstructor<T>(QScriptContext *ctx, QScriptEngine *eng, T *) \
{                                                                       \
  _Arg1 arg1 = qscriptvalue_cast<_Arg1> (ctx->argument(0));             \
    T* t = new T(arg1);                                                 \
    if (ctx->isCalledAsConstructor()) {                                 \
      QScriptValue proto = eng->defaultPrototype(qMetaTypeId<T>());     \
      QScriptValue u = eng->newQObject(ctx->thisObject(), t, QScriptEngine::AutoOwnership); \
      u.setPrototype(proto);                                            \
      return u;                                                         \
    }                                                                   \
    QScriptValue o = eng->newQObject(t, QScriptEngine::AutoOwnership);  \
    o.setPrototype(ctx->callee().property(QString::fromLatin1("prototype"))); \
    return o; \
}

Q_DECLARE_METATYPE (QEntity*)
Q_DECLARE_METATYPE (QGenericEntity*)
Q_DECLARE_METATYPE (QCollection*)
Q_DECLARE_METATYPE (QStorage*)

SoftKernelPlugin :: ~SoftKernelPlugin()
{}

static QScriptValue createSoftStorage (QScriptContext *ctx, QScriptEngine *engine)
{
  if (ctx->argumentCount() < 2) {
    ctx->throwError("Syntax error");
    return engine->nullValue();
  }
  QString const driver  = ctx->argument(0).toString();
  QString const uri     = ctx->argument(1).toString();
  QString const options = (ctx->argumentCount() >= 3 ? ctx->argument(2).toString() : "");
  auto storage = new QStorage(driver,uri,options);
  return engine->newQObject(storage, QScriptEngine::ScriptOwnership);
}

template <class T>
QScriptValue createSoftEntity (QScriptContext *ctx, QScriptEngine *engine)
{
  T *entity = nullptr;
  if (ctx->argumentCount() == 0) {
    entity = new T();
  } else if(ctx->argumentCount() == 1) {
    entity = new T(ctx->argument(0).toString());
  } else {
    ctx->throwError("Syntax error");
    return engine->nullValue();
  }
  return engine->newQObject(entity, QScriptEngine::ScriptOwnership);    
}

void SoftKernelPlugin :: registerPlugin (QScriptEngine *engine)
{
  Q_CHECK_PTR (engine);
  auto globalObj = engine->globalObject();
  auto soft = engine->newObject();

  globalObj.setProperty("soft", soft);
  globalObj.setProperty("porto", soft);
  
  registerConstructor<QStorage> (engine, soft, "Storage",   &createSoftStorage);
  registerConstructor<QEntity> (engine, soft, "Entity",     &createSoftEntity<QGenericEntity>);
  registerConstructor<QEntity> (engine, soft, "Collection", &createSoftEntity<QCollection>);
}

