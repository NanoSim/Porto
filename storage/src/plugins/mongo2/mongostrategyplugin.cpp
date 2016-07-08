#include "mongostrategyplugin.h"
#include <QCoreApplication>
#include <QTextStream>

#include "soft.h"
#include "mongostrategy.h"

MongoStrategyPlugin :: ~MongoStrategyPlugin()
{}

void MongoStrategyPlugin :: registerStrategy()
{
   auto isOk = soft::registerStorage("mongo2", soft::MongoStrategy::create);
   if (!isOk) {
      QTextStream(stderr) << "Cannot register the mongo storage strategy" << endl;
   }
}
