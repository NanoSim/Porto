#include <QtCore>
#include <QtScript>
#include "httpdaemon.h"

int main (int argc, char **argv)
{
  QCoreApplication app(argc, argv);
  QCoreApplication::setApplicationName("softweb");
  QCoreApplication::setApplicationVersion("1.1");
  QCommandLineParser commandLineParser;
  commandLineParser.setApplicationDescription("SoftWeb helper");
  commandLineParser.addHelpOption();
  commandLineParser.addVersionOption();
  QCommandLineOption configOption({"c", "config"},
				  QCoreApplication::translate("main", "Read <config> file"),
				  QCoreApplication::translate("main", "config"),
				  QString(":res/configure.js"));

  commandLineParser.addOption(configOption);
  commandLineParser.process(app);
  QString const configFile = commandLineParser.value(configOption);
  auto daemon = new HttpDaemon(configFile, &app);
  daemon->run();
  return app.exec();
}
