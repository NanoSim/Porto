#ifndef SCRIPTENGINE_H_DEF
#define SCRIPTENGINE_H_DEF
#include "softns.h"
#include "soft-kernel_export.h"

#include <string>

SOFT_BEGIN_NAMESPACE

class SOFT_KERNEL_EXPORT ScriptEngine
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

SOFT_END_NAMESPACE
#endif // SCRIPTENGINE_H_DEF
