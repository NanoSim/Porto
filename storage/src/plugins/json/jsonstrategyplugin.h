#ifndef JSONSTRATEGYPLUGIN_H_DEF
#define JSONSTRATEGYPLUGIN_H_DEF

#include <QObject>
#include "istrategyplugin.h"

class JSONStrategyPlugin : public QObject
			 , public IStrategyPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.sintef.soft/IStrategyPlugin/0.1")
  Q_INTERFACES(IStrategyPlugin)

public:
  virtual ~JSONStrategyPlugin();
  virtual void registerStrategy();
};

#endif // JSONSTRATEGYPLUGIN_H_DEF
