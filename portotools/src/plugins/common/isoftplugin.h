#ifndef ISOFTPLUGIN_H_DEF
#define ISOFTPLUGIN_H_DEF

#include <QtPlugin>

class QScriptEngine;
class ISoftPlugin
{
public:
   virtual ~ISoftPlugin() {}
   virtual void registerPlugin(QScriptEngine *engine) = 0;
};

Q_DECLARE_INTERFACE(ISoftPlugin, "org.sintef.soft/ISoftPlugin/0.1")

#endif // ISOFTPLUGIN_H_DEF
