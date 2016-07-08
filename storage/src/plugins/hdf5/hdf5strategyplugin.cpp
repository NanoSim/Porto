#include <QTextStream>
#include "hdf5strategyplugin.h"

#include "soft.h"
#include "hdf5strategy.h"

HDF5StrategyPlugin :: ~HDF5StrategyPlugin()
{}

void HDF5StrategyPlugin :: registerStrategy()
{
  auto isOk = soft::registerStorage("hdf5", soft::HDF5Strategy::create);
  if (!isOk) {
    QTextStream(stderr) << "Cannot register the hdf5 storage strategy" << endl;
  }
}
