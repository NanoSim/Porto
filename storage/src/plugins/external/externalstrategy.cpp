#include <functional>
#include <QtCore>
#include <QScopedPointer>
#include <soft.h>
#include <softc/softc-storage-plugin.h>
#include <softc/softc-datamodel-private.hpp>
#include "externalstrategy.h"
#include "externalmodel.h"

SOFT_BEGIN_NAMESPACE

const char pluginIdentify[]     = "softc_plugin_identify";
const char pluginSave[]         = "softc_plugin_save";
const char pluginLoad[]         = "softc_plugin_load";
const char pluginCapabilities[] = "softc_plugin_capabilities";
const size_t MAXLEN = 512;

namespace Singleton {
  class PluginsListSingleton
  {
    typedef int(*IdentityPrototype)(char*,int);
  public:
    PluginsListSingleton(){
      auto paths = pluginsDirectories();

      auto libFilter = [](QStringList const &files) {
	QStringList retval;
	for (auto file : files) {
	  if (QLibrary::isLibrary(file)) {
	    retval.append(file);
	  }
	}
	return retval;
      };

      for (auto const dir: paths) {
	QDir const pluginsDir (QString::fromStdString(dir));
	auto const files         = pluginsDir.entryList (QDir::Files);
	auto const filteredFiles = libFilter(files);
	for (auto const file: filteredFiles) {
	  IdentityPrototype fn = (IdentityPrototype)QLibrary::resolve(file, pluginIdentify);
	  if (fn) {
	    char id[MAXLEN];
	    fn(id, MAXLEN);
	    QString key(id);
	    QSharedPointer<QLibrary> lib(new QLibrary(pluginsDir.absolutePath() + "/" + file));
	    libraryMap.insert(key, lib);
	  }
	}
      }
    }

    QMap<QString, QSharedPointer<QLibrary>> libraryMap;
  };

  static PluginsListSingleton& get() {
    static PluginsListSingleton instance;
    return instance;
  }
}

const char *ExternalStrategy::staticMetaType = "http://sintef.no/soft/TR/external-strategy#external:0.1-SNAPSHOT-1";

const char *ExternalStrategy::metaType() const
{
  return ExternalStrategy::staticMetaType;
}

class ExternalStrategy::Private
{
  friend class ExternalStrategy;
  Private (QString const &uri, QString const &options)
    : uri(uri)
    , options (options)
  {}
  QString uri;
  QString options;
  QSharedPointer<QLibrary> extPlugin;
};

ExternalStrategy :: ExternalStrategy()
  : IStorageStrategy()
  , d(new ExternalStrategy::Private(QString(), QString()))
{}


//!
// The URI for an external strategy should contain at least the
// scheme and path in this form <scheme>://<path>, where scheme will
// invoke the factory function to select the correct plugin
ExternalStrategy :: ExternalStrategy(const char *uri, const char *options)
  : IStorageStrategy()
  , d(nullptr)
{
  QUrl url(uri);
  auto scheme = url.scheme();
  auto s = Singleton::get();
  if (!s.libraryMap.contains(scheme)) {
    throw std::runtime_error("External Storage Strategy cannot load scheme plugin");
  }
  d = new ExternalStrategy::Private(uri, options);
  d->extPlugin = s.libraryMap[scheme];
}

ExternalStrategy :: ~ExternalStrategy()
{
  delete d;
}

IDataModel *ExternalStrategy::dataModel () const
{
  IDataModel *ret = (IDataModel*)new ExternalModel();
  return ret;
}

void ExternalStrategy::store (IDataModel const *model)
{
  typedef int(*SavePrototype)(const struct _softc_datamodel_t*, const char*, const char*);
  ExternalModel const* extModel = dynamic_cast<ExternalModel const*>(model);
  Q_CHECK_PTR(extModel);
  const _softc_datamodel_t datamodel = { (IDataModel*)extModel};

  SavePrototype fn = (SavePrototype)d->extPlugin->resolve(pluginSave);
  fn(&datamodel, qPrintable(d->uri), qPrintable(d->options));
}

void ExternalStrategy::endRetrieve (IDataModel *model) const
{}

void ExternalStrategy::startRetrieve (IDataModel *model) const
{
  typedef int(*LoadPrototype)(struct _softc_datamodel_t*, const char*, const char*);
  ExternalModel *extModel = dynamic_cast<ExternalModel*>(model);
  Q_CHECK_PTR(extModel);
  struct _softc_datamodel_t datamodel;
  datamodel.ref = extModel;

  LoadPrototype fn = (LoadPrototype)d->extPlugin->resolve(pluginLoad);
  fn(&datamodel, qPrintable(d->uri), qPrintable(d->options));
}

//!
/// Creates a new storage strategy where \arg uri and \arg opts is passed
/// to the external storage plugin.
IStorageStrategy* ExternalStrategy::create(char const *uri, char const *opts)
{
  // Set correct function pointers
  auto s = new ExternalStrategy(uri, opts);
  return s;
}

SOFT_END_NAMESPACE
