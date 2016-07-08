#include <QCoreApplication>
#include <QStringList>
#include <QVariant>
#include <QDir>
#include <QUuid>
#include <QLibrary>
#include <QPluginLoader>
#include <QScopedPointer>
#include <QTextStream>
#include <QProcessEnvironment>
#include <QDebug>
#include <string>
#include <list>
#include "soft.h"

#include "storagefactory.h"
#include "istoragestrategy.h"
#include "istrategyplugin.h"
//#include "defaultstrategy.h"

static const char *storagefactoryid = "ea1ae6168c404a31bcfdd59da91c1e85";
static const char *pluginDirectory = "/plugins";
static int verboseLevel = 0;

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
    else {
      if (verboseLevel > 0) {
        QTextStream(stderr) << loader->errorString() << endl;
      }
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

static QCoreApplication* app = nullptr;
void init(int &argc, char *argv[])
{
  if (QCoreApplication::instance() == nullptr) {
    app = new QCoreApplication(argc, argv);
    auto storageFactory = new StorageFactory();
    app->setProperty(storagefactoryid, asQVariant(storageFactory));
    registerStoragePlugins();
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

std::string uuidGen()
{
  return QUuid::createUuid().toString().mid(1,36).toStdString();
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
