#include <QtCore>
#include <QtScript>
#include "httpdaemon.h"

int main (int argc, char **argv)
{
  QCoreApplication app(argc, argv);
  auto daemon = new HttpDaemon(":res/configure.js", &app);
  daemon->run();
  return app.exec();
}
