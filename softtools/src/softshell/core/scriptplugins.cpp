#include <QtCore>
#include <QtScript>
#include <Soft>

#include "isoftplugin.h"
#include "scriptplugins.h"

class ScriptPlugins :: Private
{
   friend class ScriptPlugins;

   Private() {}
   
   QList<QPluginLoader*> plugins;
};

ScriptPlugins :: ScriptPlugins(soft::ScriptEngine const &e, QObject *parent)
   : QObject(parent)
   , d( new ::ScriptPlugins::Private() )
{
   auto engine = static_cast<QScriptEngine*>(e.ref());
   load(engine);
}

ScriptPlugins :: ~ScriptPlugins()
{
}

void ScriptPlugins :: load(QScriptEngine *engine)
{
  QCoreApplication::addLibraryPath (qApp->applicationDirPath() + "/modules");
   auto const pluginPath = qApp->applicationDirPath() + "/plugins";
   QDir pluginsDir(pluginPath);
   auto files = pluginsDir.entryList(QDir::Files | QDir::Executable);
   foreach( auto const &file, files ) 
   {
      if( QLibrary::isLibrary(file) ) 
      {
	 auto loader = new QPluginLoader(pluginsDir.absoluteFilePath(file), this);
	 bool ok = loader->load();
	 if( ok ) 
	 {
	    auto pluginPtr = qobject_cast<ISoftPlugin*>(loader->instance());
	    if( pluginPtr ) {
	       pluginPtr->registerPlugin(engine);
	    }
	 }
	 else {
	    QTextStream(stderr) << loader->errorString() << endl;
	 }
      }
   }
}
