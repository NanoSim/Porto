#ifndef PROTOCALL_H_DEF
#define PROTOCALL_H_DEF

#include <QScriptValue>

template <class T, class Function>
QScriptValue protocall_void (QScriptValue const & thisObj, QObject * parent, Function fn)
{
  auto t      = q_check_ptr (qscriptvalue_cast <T*> (thisObj));
  auto engine = q_check_ptr (qobject_cast <QScriptEngine*> (parent));   
  fn(t);
  return engine->undefinedValue();    
}

template <class T, class Function>
QScriptValue protocall_sequence (QScriptValue const & thisObj, QObject * parent, Function fn)
{
  auto t      = q_check_ptr (qscriptvalue_cast <T*> (thisObj));  
  auto engine = q_check_ptr (qobject_cast <QScriptEngine*> (parent));   
  auto retval = fn (t);
  return qScriptValueFromSequence(engine, retval);  
}
 
template <class T, class Function>
QScriptValue protocall_qscriptvalue (QScriptValue const & thisObj, Function fn)
{
  auto t      = q_check_ptr (qscriptvalue_cast <T*> (thisObj));    
  return fn (t);
}

template <class T, class Function>
QScriptValue protocall (QScriptValue const & thisObj, QObject * parent, Function fn)
{
  auto t      = q_check_ptr (qscriptvalue_cast <T*> (thisObj));    
  auto engine = q_check_ptr (qobject_cast <QScriptEngine*> (parent));   
  auto retval = fn (t);
  return engine->toScriptValue (retval); 
}
  
#endif // PROTOCALL_H_DEF
