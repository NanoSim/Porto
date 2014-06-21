#include <QObject>
#include <QScopedPointer>

#include "isoftplugin.h"

class QScriptEngine;
class MongoPlugin : public QObject
		  , public ISoftPlugin
{
   Q_OBJECT
   Q_PLUGIN_METADATA(IID "org.sintef.soft/ISoftPlugin/0.1")
   Q_INTERFACES(ISoftPlugin)
   
public:
   virtual ~MongoPlugin();
   virtual void registerPlugin(QScriptEngine *engine) override;
   
}; // class MongoPlugin
