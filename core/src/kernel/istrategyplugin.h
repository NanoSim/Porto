#ifndef ISTRATEGYPLUGIN_H_DEF
#define ISTRATEGYPLUGIN_H_DEF

#include <QtPlugin>

class IStrategyPlugin
{
public:
  virtual ~IStrategyPlugin() {}
  virtual void registerStrategy() = 0;
};

Q_DECLARE_INTERFACE(IStrategyPlugin, "org.sintef.soft/IStrategyPlugin/0.1")

#endif // ISTRATEGYPLUGIN_H_DEF
