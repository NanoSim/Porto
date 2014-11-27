#ifndef REGISTER_HH
#define REGISTER_HH

template <class T>
static QScriptValue creator (QScriptContext *, QScriptEngine *engine)
{
  auto cls = new T();
  return engine->newQObject(cls, QScriptEngine::ScriptOwnership);
}

template <class T>
QScriptValue objectContructor(QScriptContext *ctx,
			      QScriptEngine *e)
{   
   auto parent = ctx->argument(0).toQObject(); 
   auto object = q_check_ptr (new T(parent));
   return e->newQObject(object, QScriptEngine::ScriptOwnership);   
}

template <class P, class T>
static void registerPrototype (QScriptEngine *engine)
{
   auto prototype = q_check_ptr (new P (engine));
   engine->setDefaultPrototype (qMetaTypeId <T*> (),
				engine->newQObject (prototype));
}

template <class T, class Fn>
static void registerConstructor (QScriptEngine *engine,
				 QString const & name,
				 Fn construct)
{
   auto ctor    = engine->newFunction(construct);
   auto metaObj = engine->newQMetaObject(&T::staticMetaObject, ctor);
   engine->globalObject().setProperty(name, metaObj);
}


#endif // REGISTER_HH
