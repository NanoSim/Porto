#include <system_error>
#include <cassert>
#include <QtCore>
#include <Soft>
#include "portooutput.h"
#include "reference.hxx"
#include "file.hxx"
#include "chemkinreaction.hxx"
#include "common.h"

PortoOutput :: PortoOutput(soft::Collection *collection, QObject *parent)
  : QObject(parent)
  , collection(collection)
{}

PortoOutput :: ~PortoOutput()
{}

void PortoOutput :: run()
{
  std::string metaName, metaVersion, metaNamespace, uuid;
  soft::Storage storage(const_global_driver, const_global_uri, const_global_options);
  collection->findEntity("dftBoundayFile", metaName, metaVersion, metaNamespace, uuid);
  QScopedPointer<soft::File> thermoFile(new soft::File(uuid));
  storage.load(thermoFile.data());
  assert(nullptr != thermoFile);

  // Setup for writing the data contents to a file
  QFileInfo thermoFileInfo(QDir::current(), QString::fromStdString(thermoFile->filename));
  {
    QFile outFile(thermoFileInfo.absoluteFilePath());
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qWarning() << "Failed to write thermo file." << outFile.errorString();
        return;
    }
    // Write blob to file
    outFile.write((const char*)thermoFile->data.data(), (quint64)thermoFile->data.size());
    outFile.close();
  }
  // Verify that the termo file has indeed been created
  assert(thermoFileInfo.exists());
  
  auto chemfileRef = dynamic_cast<soft::Reference const*>(collection->findInstance("surfaceChemkinRef"));
  assert(nullptr != chemfileRef);
  // Verify that the surface file exists
  QFileInfo chemicalInfo(QString::fromStdString(chemfileRef->uri));
  assert(chemicalInfo.exists());

  // Copy the SurfaceChemkin to the same working directory we have thermo.dat
  QFile::copy(chemicalInfo.absoluteFilePath(), thermoFileInfo.absolutePath()+"/"+chemicalInfo.fileName());  

  // Run the external storage on the Chemkin file to read data into the entities.
  QString url = QString("chemkin://%1?chem=%2&thermo=%3")
    .arg(thermoFileInfo.absolutePath())
    .arg(chemicalInfo.fileName())
    .arg(thermoFileInfo.fileName());
  QScopedPointer<soft::Storage> chemkinStorage (new soft::Storage("external", qPrintable(url), ""));

  // Read collection from the external storage
  QScopedPointer<soft::Collection> chemkinCollection(new soft::Collection());
  auto chemkinReaction0 = new soft::Chemkinreaction(0,0,0,0,0);
  auto chemkinReaction1 = new soft::Chemkinreaction(0,0,0,0,0);
  chemkinCollection->attachEntity("reaction_0", chemkinReaction0);
  chemkinCollection->attachEntity("reaction_1", chemkinReaction1);
  chemkinCollection->setName("ChemkinCollection");
  chemkinCollection->setVersion("1.0-DEMO");
  chemkinStorage->load(chemkinCollection.data());
  storage.save(chemkinReaction0);
  storage.save(chemkinReaction1);
  storage.save(chemkinCollection.data());
  collection->addRelation("reactiondata", "has-id", chemkinReaction0->id());
  collection->addRelation("reactiondata", "has-id", chemkinReaction1->id());

  // Attach the chemkin collection to out main collection and write back all changes
  collection->registerEntity("chemkinCollection", chemkinCollection.data());
  storage.save(collection);  
  
  emit finished();
}

