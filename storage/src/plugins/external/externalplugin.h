#ifndef EXTERNALPLUGIN_H_DEF
#define EXTERNALPLUGIN_H_DEF

#include <QObject>
#include "istrategyplugin.h"

class ExternalStrategyPlugin : public QObject
			     , public IStrategyPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.sintef.soft/IStrategyPlugin/0.1")
  Q_INTERFACES(IStrategyPlugin)

public:
  virtual ~ExternalStrategyPlugin();
  virtual void registerStrategy();
};


#endif /* EXTERNALPLUGIN_H_DEF */
