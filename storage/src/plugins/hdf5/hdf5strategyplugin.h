#ifndef HDF5STRATEGYPLUGIN_H_DEF
#define HDF5STRATEGYPLUGIN_H_DEF

#include <QObject>
#include "istrategyplugin.h"

class HDF5StrategyPlugin : public QObject
			 , public IStrategyPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.sintef.soft/IStrategyPlugin/0.1")
  Q_INTERFACES(IStrategyPlugin)

public:
  virtual ~HDF5StrategyPlugin();
  virtual void registerStrategy();
};

#endif // HDF5STRATEGYPLUGIN_H_DEF
