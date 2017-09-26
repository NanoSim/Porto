#include <QtCore>
#include <storage.h>
#include "portoinput.h"
#include <reference.hxx>
#include <file.hxx>
#include "common.h"

PortoInput :: PortoInput(soft::Collection *collection, QObject *parent)
  : QObject(parent)
  , collection(collection)
{}

PortoInput :: ~PortoInput()
{}
                        
void PortoInput :: run()
{
  soft::Storage storage(const_global_driver, const_global_uri, const_global_options);
  std::string metaName, metaVersion, metaNamespace, uuid;
  
  collection->findEntity("dftPath", metaName, metaVersion, metaNamespace, uuid);
  auto dftPath = new soft::Reference(uuid);
  storage.load(dftPath);
  
  collection->attachEntity("dftPath", dftPath);
  emit finished();
}
