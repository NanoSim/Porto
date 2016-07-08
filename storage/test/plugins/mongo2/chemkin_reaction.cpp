
#include <stdexcept>
#include <idatamodel.h>
#include "chemkin_reaction.h"

SOFT_BEGIN_NAMESPACE
#define NOT_IMPLEMENTED throw std::runtime_error("Not implemented");

struct Chemkin_reactionDims
{
  StdUInt nreactants;
  StdUInt nproducts;
  StdUInt ntroe;
  StdUInt nenhancement_factors;
  StdUInt nplog;
};

class Chemkin_reaction :: Private
{
  friend class Chemkin_reaction;
  Private(StdUInt nreactants,StdUInt nproducts,StdUInt ntroe,StdUInt nenhancement_factors,StdUInt nplog)
    : dims {nreactants,nproducts,ntroe,nenhancement_factors,nplog}
  {} 
  Private()
  {}
  Chemkin_reactionDims dims;
};

Chemkin_reaction :: Chemkin_reaction (StdUInt nreactants,StdUInt nproducts,StdUInt ntroe,StdUInt nenhancement_factors,StdUInt nplog)
: IEntity()
, d(new Chemkin_reaction::Private(nreactants,nproducts,ntroe,nenhancement_factors,nplog))
{}

Chemkin_reaction :: Chemkin_reaction(std::string const &id)
: IEntity(id)
, d(new Chemkin_reaction::Private())
{}

Chemkin_reaction :: Chemkin_reaction(const IEntity *ptr)
: IEntity(ptr)
, d(new Chemkin_reaction::Private())
{}

Chemkin_reaction :: Chemkin_reaction(IDataModel const *dm)
: IEntity()
, d(new Chemkin_reaction::Private())
{
  load(dm);
}

Chemkin_reaction :: ~Chemkin_reaction ()
{
  delete d;
}

IEntity* Chemkin_reaction :: create (std::string const &uuid)
{
  NOT_IMPLEMENTED	
    return nullptr;	
}

void Chemkin_reaction :: save (IDataModel *dataModel) const
{
  dataModel->setId(id());
  dataModel->appendDimension("nreactants", d->dims.nreactants);
  dataModel->appendDimension("nproducts", d->dims.nproducts);
  dataModel->appendDimension("ntroe", d->dims.ntroe);
  dataModel->appendDimension("nenhancement_factors", d->dims.nenhancement_factors);
  dataModel->appendDimension("nplog", d->dims.nplog);
  dataModel->appendStringArray ("reactants", reactants);
  dataModel->appendStringArray ("products", products);
  dataModel->appendBool ("third_body", third_body);
  dataModel->appendDouble ("A", A);
  dataModel->appendDouble ("b", b);
  dataModel->appendDouble ("Ea", Ea);
  dataModel->appendDouble ("A_low", A_low);
  dataModel->appendDouble ("b_low", b_low);
  dataModel->appendDouble ("Ea_low", Ea_low);
  dataModel->appendDoubleArray ("troe", troe);
  dataModel->appendStringArray ("enhancement_species", enhancement_species);
  dataModel->appendDoubleArray ("enhancement_factors", enhancement_factors);
  dataModel->appendDoubleArray ("P_plog", P_plog);
  dataModel->appendDoubleArray ("A_plog", A_plog);
  dataModel->appendDouble ("b_plog", b_plog);
  dataModel->appendDouble ("Ea_plog", Ea_plog);
}

void Chemkin_reaction :: load (IDataModel const *dataModel)
{
  setId(dataModel->id());
  dataModel->getDimension("nreactants", d->dims.nreactants);
  dataModel->getDimension("nproducts", d->dims.nproducts);
  dataModel->getDimension("ntroe", d->dims.ntroe);
  dataModel->getDimension("nenhancement_factors", d->dims.nenhancement_factors);
  dataModel->getDimension("nplog", d->dims.nplog);
  dataModel->getStringArray ("reactants", reactants);
  dataModel->getStringArray ("products", products);
  dataModel->getBool ("third_body", third_body);
  dataModel->getDouble ("A", A);
  dataModel->getDouble ("b", b);
  dataModel->getDouble ("Ea", Ea);
  dataModel->getDouble ("A_low", A_low);
  dataModel->getDouble ("b_low", b_low);
  dataModel->getDouble ("Ea_low", Ea_low);
  dataModel->getDoubleArray ("troe", troe);
  dataModel->getStringArray ("enhancement_species", enhancement_species);
  dataModel->getDoubleArray ("enhancement_factors", enhancement_factors);
  dataModel->getDoubleArray ("P_plog", P_plog);
  dataModel->getDoubleArray ("A_plog", A_plog);
  dataModel->getDouble ("b_plog", b_plog);
  dataModel->getDouble ("Ea_plog", Ea_plog);
}

std::vector<std::string> Chemkin_reaction :: dimensions() const
{
  return std::vector<std::string>({"nreactants","nproducts","ntroe","nenhancement_factors","nplog"});
}

int Chemkin_reaction :: getDimensionSize(std::string const &) const
{
  NOT_IMPLEMENTED;
}

SOFT_END_NAMESPACE
