#ifndef SOFT_H_DEF
#define SOFT_H_DEF
#include "config.h"
#include "softns.h"
#include "soft-kernel_export.h"

#include <string>
#include <list>
#include <memory>

SOFT_BEGIN_NAMESPACE

class IStorageStrategy;

void SOFT_KERNEL_EXPORT init(int &argc, char *argv[]);
std::string SOFT_KERNEL_EXPORT applicationDirPath();
std::string SOFT_KERNEL_EXPORT uuidGen(); /* TODO: Move this to a utils header */
std::string SOFT_KERNEL_EXPORT uuidFromEntity(const char *name, const char *version, const char *ns);
std::list<std::string> SOFT_KERNEL_EXPORT arguments();
int SOFT_KERNEL_EXPORT storageDriverCount();
std::list<std::string> SOFT_KERNEL_EXPORT registeredStorageDrivers();
bool SOFT_KERNEL_EXPORT registerStorage(const char *name, IStorageStrategy*(*createFunc)(const char*, const char *options));
std::shared_ptr<IStorageStrategy> SOFT_KERNEL_EXPORT create(const char *name, const char *uri, const char *options);
IStorageStrategy SOFT_KERNEL_EXPORT *createStrategy(const char *name, const char *uri, const char *options);
std::list<std::string> SOFT_KERNEL_EXPORT pluginsDirectories();

SOFT_END_NAMESPACE

#endif // SOFT_H_DEF
