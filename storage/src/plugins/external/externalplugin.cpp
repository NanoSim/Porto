#include <QTextStream>
#include "externalplugin.h"

#include "externalstrategy.h"
#include "soft.h"

ExternalStrategyPlugin :: ~ExternalStrategyPlugin()
{}

void ExternalStrategyPlugin :: registerStrategy()
{
  auto isOk = soft::registerStorage("external", soft::ExternalStrategy::create);
  if (!isOk) {
    QTextStream(stderr) << "Cannot register the external storage strategy" << endl;
  }
}
