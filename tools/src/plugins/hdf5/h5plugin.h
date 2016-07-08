#include <QObject>
#include "isoftplugin.h"

class QScriptEngine;
class H5Plugin : public QObject
	       , ISoftPlugin
{
   Q_OBJECT
   Q_PLUGIN_METADATA(IID "org.sintef.soft/ISoftPlugin/0.1")
   Q_INTERFACES(ISoftPlugin)
public:
   virtual ~H5Plugin();
   virtual void registerPlugin (QScriptEngine *engine) override;
};
