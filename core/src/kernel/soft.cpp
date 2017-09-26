#include <QCoreApplication>
#include <QStringList>
#include <QVariant>
#include <QDir>
#include <QUuid>
#include <QLibrary>
#include <QPluginLoader>
#include <QScopedPointer>
#include <QTextStream>
#include <QSettings>
#include <QProcessEnvironment>
#include <QDebug>
#include <string>
#include <list>
#include "soft.h"

#include "storagefactory.h"
#include "istoragestrategy.h"
#include "istrategyplugin.h"

#ifdef _WIN32
  static const char *verbosityLevel = "HKEY_CURRENT_USER\\SINTEF\\SOFT5\\core\\verbosity-level";
#else
  static const char *verbosityLevel = "core/verbosity-level";
#endif
static const char *storagefactoryid = "ea1ae6168c404a31bcfdd59da91c1e85";
static const char *pluginDirectory = "/plugins";


SOFT_BEGIN_NAMESPACE

namespace {
   template <class T>
   static T* asPtr (QVariant v)
   {
      return (T*)v.value<void*>();
   }

   template <class T>
   static QVariant asQVariant(T *ptr)
   {
      return qVariantFromValue((void*)ptr);
   }
}

std::list<std::string> registeredStorageDrivers()
{
  if (QCoreApplication::instance() == nullptr) {
    return std::list<std::string>();
  }

  auto *factory = asPtr<StorageFactory>(qApp->property(storagefactoryid));
  std::list<std::string> retval;
  for (auto i = 0; i < factory->driverCount(); ++i) {
    retval.push_back(factory->driverName(i));
  }

  return retval;
}

int storageDriverCount()
{
  auto driver = registeredStorageDrivers();
  return driver.size();
}

static bool registerPlugin(QString const &file)
{
  if (QLibrary::isLibrary (file)) {
    QScopedPointer<QPluginLoader> loader (new QPluginLoader (file));
    auto isOk   = loader->load();
    if (isOk) {
      auto pluginPtr = qobject_cast<IStrategyPlugin*>(loader->instance());
      if (pluginPtr) {
        pluginPtr->registerStrategy();
        return true;
      }
    }
    else if (qApp->property(verbosityLevel).toInt() > 0) {
      QTextStream(stderr) << loader->errorString() << endl;
    }
  }
  return false;
}

static QList<QDir> pluginsDirList()
{
  QDir const configDir(QString("%1/storage").arg(SOFTBASE));
  QList<QDir> list = QList<QDir>() << configDir;
  if (QProcessEnvironment::systemEnvironment().contains("SOFTBASE")) {
    auto const customDirString = QProcessEnvironment::systemEnvironment().value("SOFTBASE") + "/storage";
    QDir const customDir(customDirString);
    list << customDir;
  }
  if (QProcessEnvironment::systemEnvironment().contains("SOFTSTORAGE")) {
    auto const customDirString = QProcessEnvironment::systemEnvironment().value("SOFTSTORAGE");
    auto const customDirStringPaths = customDirString.split(":");
    for (auto p : customDirStringPaths) {
      QDir const customDir(p);
      list << customDir;
    }
  }

  if (qApp->arguments().count() > 1 ) {
    auto const argPaths = qApp->arguments().at(1).split(":");
    for (auto path : argPaths) {
      QDir const d(path);
      list << d;
    }
  }

  return list;
}

std::list<std::string> pluginsDirectories()
{
  std::list<std::string> ret;
  for(const auto dir: pluginsDirList()) {
    std::string path = dir.absolutePath().toStdString();
    ret.push_back(path);
  }
  return ret;
}

static bool registerStoragePlugins()
{
  bool isOk = false;
  for(auto pluginsDir : pluginsDirList()) {
    auto const files = pluginsDir.entryList (QDir::Files);
    foreach (auto const &file, files) {
      isOk = registerPlugin (pluginsDir.absoluteFilePath(file));
    }
  }
  return isOk;
}

static void writeSettings()
{
  QSettings settings;
  settings.setValue(verbosityLevel, qApp->property(verbosityLevel).toInt());
}

static QCoreApplication* app = nullptr;
void init(int &argc, char *argv[])
{
  if (QCoreApplication::instance() == nullptr) {
    QCoreApplication::setOrganizationName("SINTEF");
    QCoreApplication::setOrganizationDomain("www.sintef.no");
    QCoreApplication::setApplicationName("SOFT5");
    QSettings settings;    
    app = new QCoreApplication(argc, argv);    
    auto storageFactory = new StorageFactory();
    app->setProperty(verbosityLevel, settings.value(verbosityLevel, 0).toInt());
    app->setProperty(storagefactoryid, asQVariant(storageFactory));
    registerStoragePlugins();
    writeSettings();
  }
}

bool registerStorage(const char *name, IStorageStrategy*(*createFunc)(const char*, const char*))
{
  if (QCoreApplication::instance() == nullptr)
    return false;

  auto *factory = asPtr<StorageFactory>(qApp->property(storagefactoryid));
  return factory->registerStrategy(name, createFunc);
}

std::shared_ptr<IStorageStrategy> create(const char *name, const char *uri, const char *options)
{
  if (QCoreApplication::instance() == nullptr)
    return std::shared_ptr<IStorageStrategy>();

  auto *factory = asPtr<StorageFactory>(qApp->property(storagefactoryid));
  std::shared_ptr<IStorageStrategy> retval (factory->create(name, uri, options));
  return retval;
}

IStorageStrategy* createStrategy(const char *name, const char *uri, const char *options)
{
  if (QCoreApplication::instance() == nullptr)
    return nullptr;

  auto *factory = asPtr<StorageFactory>(qApp->property(storagefactoryid));
  return factory->create(name, uri, options);
}

std::string applicationDirPath()
{
   return QCoreApplication::applicationDirPath().toStdString();
}

/*!
  Returns an url constructed from \a name, \a version and \a ns.
 */
QString urlFromEntity(const char *name, const char *version, const char *ns)
{
  return QString("%1/%2-%3").arg(ns).arg(name).arg(version);
}

/*!
  Returns a new unique random UUID.
 */
std::string uuidGen()
{
  return QUuid::createUuid().toString().mid(1,36).toStdString();
}

/*!
  Returns an UUID generated from a MD5 hash of \a url.
 */
std::string uuidFromUrl(QString url)
{
  // NameSpace_URL from RFC4122
  QUuid nsUrl(0x6ba7b811, 0x9dad, 0x11d1, 0x80, 0xb4,
              0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8);
  return QUuid::createUuidV3(nsUrl, url).toString().mid(1,36).toStdString();
}

/*!
  Returns an UUID generated from \a name, \a version and \a ns.
  Can be used as an alternative identifier of entities.
 */
std::string uuidFromEntity(const char *name, const char *version, const char *ns)
{
  return uuidFromUrl(urlFromEntity(name, version, ns));
}

std::list<std::string> arguments()
{
   std::list<std::string> retval;
   foreach(auto argument, QCoreApplication::arguments()) {
      retval.push_back (argument.toStdString());
   }
   return retval;
}

SOFT_END_NAMESPACE
