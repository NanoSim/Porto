#include "soft.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>

SOFT_BEGIN_NAMESPACE

static QCoreApplication* app = nullptr;

void init(int &argc, char *argv[])
{
   if (!app) {
      app = new QCoreApplication(argc, argv);
   }
}

std::string applicationDirPath()
{
   return QCoreApplication::applicationDirPath().toStdString();
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
