#ifndef SOFT_KERNEL_PLUGIN_H_DEF
#define SOFT_KERNEL_PLUGIN_H_DEF

#include <QObject>
#include <isoftplugin.h>

class SoftKernelPlugin : public QObject
		       , public ISoftPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.sintef.soft/ISoftPlugin/0.1")
  Q_INTERFACES(ISoftPlugin)
 

public:
  virtual ~SoftKernelPlugin();
  virtual void registerPlugin (QScriptEngine *) override;
public slots:
};

#endif // SOFT_KERNEL_PLUGIN_H_DEF
