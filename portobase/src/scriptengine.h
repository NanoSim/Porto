#ifndef SCRIPTENGINE_H_DEF
#define SCRIPTENGINE_H_DEF
#include "portons.h"

#include <string>

PORTO_BEGIN_NAMESPACE

class ScriptEngine
{
public:
   ScriptEngine();
   virtual ~ScriptEngine();

   std::string evaluate(std::string const &program, std::string const &fileName = std::string(), int lineNumber = 1);

   void *ref() const;

private:
   class Private;
   Private *d;
};

PORTO_END_NAMESPACE
#endif // SCRIPTENGINE_H_DEF
