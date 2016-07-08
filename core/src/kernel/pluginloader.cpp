#include <QPluginLoader>
#include <QScopedPointer>
#include <QStringList>
#include <QLibrary>
#include <QDir>

#include "pluginloader.h"

SOFT_BEGIN_NAMESPACE

class PluginLoader::Private
{
  friend class PluginLoader;
  QStringList files;
  QStringList paths;
};

static QStringList libraryFilter(QStringList const &files)
{
  QStringList retval;
  for (auto file : files) {
    if (QLibrary::isLibrary(file)) {
      retval.append(file);
    }
  }
  return retval;
}

PluginLoader :: PluginLoader()
  :d (new PluginLoader::Private())
{
  Q_CHECK_PTR (d);
}

PluginLoader :: ~PluginLoader()
{
  delete d;
}

/*!
  Adds a \a path to the current list of plugin paths.
  \a path directory to add to the current load path.
 */
bool PluginLoader :: addLoadPath (const char *path)
{
  auto const p = QString::fromLocal8Bit(path);
  QDir dir(p);
  if (dir.exists()) {
    d->paths.append(p);
    return true;
  }
  
  return false;
}

bool PluginLoader :: load (const char *IID, std::function<bool(::QObject*)> fn)
{
  for (auto const dir: d->paths) {
    QDir const pluginsDir (dir);
    auto const files = pluginsDir.entryList (QDir::Files);
    auto const filteredFiles = libraryFilter (files);
    for (auto const file: filteredFiles) {
      QScopedPointer<QPluginLoader> loader (new QPluginLoader (pluginsDir.absoluteFilePath(file)));
      auto const isOk = loader->load();
      if (isOk) {
	auto const jsonObj = loader->metaData();
	if (jsonObj.value("IID") == QString::fromLocal8Bit(IID)) {
	  return fn (loader->instance());	  
	}
      }
    }	 
  }
  return false;
}

SOFT_END_NAMESPACE
