#ifndef PORTO_H_DEF
#define PORTO_H_DEF
#include "portons.h"
#include <string>
#include <list>

PORTO_BEGIN_NAMESPACE

void init(int &argc, char *argv[]);
std::string applicationDirPath();
std::list<std::string> arguments();

PORTO_END_NAMESPACE

#endif // PORTO_H_DEF
