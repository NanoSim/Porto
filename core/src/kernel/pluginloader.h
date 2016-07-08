#ifndef PLUGINLOADER_H_DEF
#define PLUGINLOADER_H_DEF

#include <functional>
#include "softns.h"

class QObject;
SOFT_BEGIN_NAMESPACE

class PluginLoader
{
public:
  PluginLoader();
  virtual ~PluginLoader();
  bool addLoadPath (const char *path);
  bool load (const char *IID, std::function<bool(::QObject*)> fn);

private:
  class Private;
  Private *d;
};

SOFT_END_NAMESPACE
#endif // PLUGINLOADER_H_DEF
