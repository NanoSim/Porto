#include <cstring>
#include <iostream>
#include <memory>
#include <QtCore>
#include <Soft>
#include <softc/softc-datamodel-private.hpp>
#include <chemkinReader.h>
#include "chemkinreaction.hxx"
#include "chemkin.h"

static char const defaultChemFile[]  = "SurfaceChemkin.inp";
static char const defaultThermFile[] = "thermo.dat";

int softc_plugin_identify (char* name, int maxlen) {
  const char plugin_name[] = "chemkin";
  const size_t len = std::min<size_t>(sizeof(plugin_name), maxlen);
  strncpy(name, plugin_name, len);
  name[len - 1] = '\0';
  return SOFTC_STATUS_OK;
}

int softc_plugin_capabilities ()
{
  return SOFTC_CAPABILITY_READ;
}

// Loads from an URI into a datamodel
int softc_plugin_load (softc_datamodel_t* datamodel, char const *uri, char const* options)
{
  using namespace soft;
  QUrl        const loc(uri);
  QUrlQuery   const query(uri);
  QFileInfo   const chemFileInfo(loc.path(), query.hasQueryItem("chem") ? query.queryItemValue("chem") : defaultChemFile);
  QFileInfo   const thermFileInfo(loc.path(), query.hasQueryItem("thermo") ? query.queryItemValue("thermo"): defaultThermFile);
  std::string const chemfile  = chemFileInfo.absoluteFilePath().toStdString();
  std::string const thermfile = thermFileInfo.absoluteFilePath().toStdString();

  //  soft::Collection coll(datamodel->ref->id());
  soft::Collection coll(datamodel->ref);

  IO::ChemkinReader chemkinReader(chemfile, thermfile);
  chemkinReader.read();
  auto reactions = chemkinReader.reactions();
  StdDoubleArray A; // preexponential factor
  StdDoubleArray b; 
  StdDoubleArray Ea; 

  StdUInt nreactants = 0;
  StdUInt nproducts = 0;
  StdUInt ntroe = 0;
  StdUInt nenhancement_factors = 0;
  StdUInt nplog = 0;
  int ridx = 0;

  for (auto reaction: reactions) {
    
    auto arrhenius = reaction.getArrhenius();
    auto reactants = reaction.getReactants();
    auto products = reaction.getProducts();
    auto low = reaction.getLOW();
    auto troe = reaction.getTROE();

    std::unique_ptr<Chemkinreaction> chemkinReaction(new Chemkinreaction (reactants.size(), 
									  products.size(), 
									  troe.size(), 
									  0 /* nenhancement_factors */,
									  0 /* nplog */));
    
    for(auto reactant: reactants) {
      chemkinReaction->reactants.push_back(reactant.first);
    }
    for(auto product: products) {
      chemkinReaction->products.push_back(product.first);
    }    
    
    chemkinReaction->third_body = reaction.hasThirdBody();
    chemkinReaction->A = arrhenius.A;
    chemkinReaction->b = arrhenius.n;
    chemkinReaction->Ea = arrhenius.E;
    std::cout << chemkinReaction->A << std::endl;

    if (reaction.hasLOW()) {
      if (!low.size() == 3) {
	std::cerr << "Chemkin Reaction Invalid LOW" << std::endl;
	return SOFTC_STATUS_FAILURE;
      }
      chemkinReaction->A_low = low[0];
      chemkinReaction->b_low = low[1];
      chemkinReaction->Ea_low = low[2];
    } else {
      if (!low.size() == 0) {
	std::cerr << "Chemkin Reaction Invalid LOW" << std::endl;
	return SOFTC_STATUS_FAILURE;
      }		     
    }

    for(auto tro: troe) {
      chemkinReaction->troe.push_back(tro);
    }

    auto const label = QString("reaction_%1").arg(QString::number(ridx)).toStdString();
    if (!datamodel->ref->getModel(label.c_str())) {
      coll.attachEntity(label, chemkinReaction.get());
    }
    ridx++;
  }
  coll.save(datamodel->ref);

  return SOFTC_STATUS_OK;
}

int softc_plugin_save(softc_datamodel_t const* datamodel, const char* uri, const char* options)
{  
  return SOFTC_STATUS_FAILURE;
}

