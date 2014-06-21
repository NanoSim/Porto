#ifndef AUTOMODULE_H_DEF
#define AUTOMODULE_H_DEF

class QScriptEngine;
class ScriptEngine
{
public:
  static QScriptEngine *globalEngine();

protected:
  ScriptEngine() {}
  
private:
  static QScriptEngine *instance;
};

#endif // AUTOMODULE_H_DEF
