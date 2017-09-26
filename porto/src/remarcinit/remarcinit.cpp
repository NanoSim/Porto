#include <cstring>
#include <iostream>
#include <QtCore>
#include <Soft>
#include <softc/softc-datamodel-private.hpp>
#include <remarcReader.h>
#include "remarcsetup.hxx"

#include "remarcinit.h"

static char const defaultRemarcInFile[] = "remarc.in";

int softc_plugin_identify (char* name, int maxlen) {
  const char plugin_name[] = "remarc-init";
  const size_t len = std::min<size_t>(sizeof(plugin_name), maxlen);
  strncpy(name, plugin_name, len);
  name[len - 1] = '\0';
  return SOFTC_STATUS_OK;
}

int softc_plugin_capabilities ()
{
  return SOFTC_CAPABILITY_READ|SOFTC_CAPABILITY_WRITE;
}

int softc_plugin_load (softc_datamodel_t* datamodel, char const *uri, char const* options)
{
  using namespace soft;
  QUrl const loc(uri);
  QString filePath = loc.path();
  IO::RemarcReader config(filePath.toStdString());

  // Read from file and populate the entity with data
  config.read();
  soft::Remarcsetup entity;  
  entity.T_min = config.T_min;
  entity.T_max = config.T_max;
  entity.Delta_T = config.dt;
  entity.barrierHeight = config.bh;
  entity.adsoptionEnergy = config.ae;
  entity.p = config.gp;
  entity.useFreqToCalcRateConstants = config.frc;
  entity.useFreqToCleanSurface = config.fcs;
  entity.useQuantum = config.qvp;
  entity.rateConstPrefactor = config.prc;
  entity.desorptionRatePrefactor = config.pdr;

  // Store entity state into the datamodel
  entity.save(datamodel->ref);
  
  return SOFTC_STATUS_OK;
}

int softc_plugin_save(softc_datamodel_t const* datamodel, const char* uri, const char* options)
{
  using namespace soft;
  QUrl loc(uri);
  QString filePath = loc.path();
  IO::RemarcReader config(filePath.toStdString());
  soft::Remarcsetup entity;
  // Read data from the datamodel into the entity
  entity.load(datamodel->ref);

  // Setup for writing to file
  config.T_min = entity.T_min;
  config.T_max = entity.T_max;
  config.dt = entity.Delta_T;
  config.bh = entity.barrierHeight;
  config.ae = entity.adsoptionEnergy;
  config.gp = entity.p;
  config.frc = entity.useFreqToCalcRateConstants;
  config.fcs = entity.useFreqToCleanSurface;
  config.qvp = entity.useQuantum;
  config.prc = entity.rateConstPrefactor;
  config.pdr = entity.desorptionRatePrefactor;

  // Write file
  config.write();
  
  return SOFTC_STATUS_OK;
}
