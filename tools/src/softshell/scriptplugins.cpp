#include <QtCore>
#include <QtScript>
#include <Soft>

#include "softns.h"
#include "isoftplugin.h"
#include "scriptplugins.h"

class ScriptPlugins :: Private
{
   friend class ScriptPlugins;

   Private() {}
   QMap<QString, QPluginLoader*>pluginsMap;
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
  while (!d->pluginsMap.empty()) {
    auto const key = d->pluginsMap.firstKey();
    auto v = d->pluginsMap.take(key);
    (*v).unload();
  }
}

void ScriptPlugins :: load(QScriptEngine *engine)
{
  auto const softpath = QProcessEnvironment::systemEnvironment().value("SOFTBASE", SOFTBASE);
  QStringList pluginList;
  QCoreApplication::addLibraryPath (softpath + "/modules");
  auto const pluginPath = softpath + "/plugins";
  QDir pluginsDir(pluginPath);
  auto files = pluginsDir.entryList(QDir::Files);
  foreach( auto const &file, files ) {
    if( QLibrary::isLibrary(file) ) {
      auto loader = new QPluginLoader(pluginsDir.absoluteFilePath(file), this);
      auto const ok = loader->load();
      if( ok ) {
        d->pluginsMap.insert(file, loader);
        auto const jsonObj = loader->metaData();
        if (jsonObj.value("IID") == "org.sintef.soft/ISoftPlugin/0.1") {
          auto pluginPtr = qobject_cast<ISoftPlugin*>(loader->instance());
          if( pluginPtr ) {
            pluginPtr->registerPlugin(engine);
            pluginList << file;
          }
          else {
            QTextStream(stdout) << "failed to register: " << loader->errorString() << endl;
          }
        }
      }
    }
  }
  auto const l = qScriptValueFromSequence(engine, pluginList);
  engine->globalObject().setProperty("__script_plugins__", l);
}
