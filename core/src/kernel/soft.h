#ifndef SOFT_H_DEF
#define SOFT_H_DEF
#include "config.h"
#include "softns.h"

#include <string>
#include <list>
#include <memory>

SOFT_BEGIN_NAMESPACE

class IStorageStrategy;

void                              init(int &argc, char *argv[]);
std::string                       applicationDirPath();
std::string                       uuidGen(); /* TODO: Move this to a utils header */
std::list<std::string>            arguments();
int                               storageDriverCount();
std::list<std::string>            registeredStorageDrivers();
bool                              registerStorage(const char *name, IStorageStrategy*(*createFunc)(const char*, const char *options));
std::shared_ptr<IStorageStrategy>  create(const char *name, const char *uri, const char *options);
IStorageStrategy*                  createStrategy(const char *name, const char *uri, const char *options);
std::list<std::string>            pluginsDirectories();

SOFT_END_NAMESPACE

#endif // SOFT_H_DEF
