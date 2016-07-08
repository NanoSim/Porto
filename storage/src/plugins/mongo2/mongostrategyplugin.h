#ifndef MONGOSTRATEGYPLUGIN_H_DEF
#define MONGOSTRATEGYPLUGIN_H_DEF

#include <QObject>
#include "istrategyplugin.h"

class MongoStrategyPlugin : public QObject
			  , public IStrategyPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.sintef.soft/IStrategyPlugin/0.1")
  Q_INTERFACES(IStrategyPlugin)

public:
  virtual ~MongoStrategyPlugin();
  virtual void registerStrategy();
};

#endif // MONGOSTRATEGYPLUGIN_H_DEF
