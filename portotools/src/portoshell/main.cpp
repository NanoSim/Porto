#include <Porto>

#include "utils.h"
#include "portoshell.h"
#include "scriptplugins.h"

int main(int argc, char *argv[])
{  
  initScriptResource();
  porto::init(argc, argv);
  porto::ScriptEngine engine;

  Utils utilities (engine);
  ScriptPlugins plugins(engine);; 
  return startRepl (engine);
}
