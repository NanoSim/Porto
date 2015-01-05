#ifndef SOFT_H_DEF
#define SOFT_H_DEF
#include "softns.h"
#include <string>
#include <list>

SOFT_BEGIN_NAMESPACE

void init(int &argc, char *argv[]);
std::string applicationDirPath();
std::list<std::string> arguments();

SOFT_END_NAMESPACE

#endif // SOFT_H_DEF
