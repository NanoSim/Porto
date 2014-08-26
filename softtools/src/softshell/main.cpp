#include <Soft>

#include "utils.h"
#include "softshell.h"
#include "scriptplugins.h"

int main(int argc, char *argv[])
{
  initScriptResource();
  soft::init(argc, argv);
  soft::ScriptEngine engine;

  Utils utilities (engine);
  ScriptPlugins plugins(engine);; 
  return startRepl (engine);
}
