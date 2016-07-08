
#ifndef CHEMKIN_REACTION_ENTITY_H_DEF
#define CHEMKIN_REACTION_ENTITY_H_DEF

#include <softns.h>
#include <softtypes.h>
#include <ientity.h>


SOFT_BEGIN_NAMESPACE

class Chemkin_reaction : public IEntity
{
 public:
  SOFT_ENTITY_METADATA(SOFT_META_STRING(Chemkin_reaction), SOFT_META_STRING(eu.nanosim.vasp), SOFT_META_STRING(0.1))
    Chemkin_reaction (StdUInt nreactants, StdUInt nproducts, StdUInt ntroe, StdUInt nenhancement_factors, StdUInt nplog);
  explicit Chemkin_reaction(std::string const &);
  explicit Chemkin_reaction(IEntity const *);
  explicit Chemkin_reaction(IDataModel const *);  
  virtual ~Chemkin_reaction();

  static IEntity* create (std::string const &uuid = std::string());
  virtual void save (IDataModel *) const override;
  virtual void load (IDataModel const *) override;

  virtual std::vector<std::string> dimensions() const override;
  virtual int getDimensionSize(std::string const &) const override;

  StdStringList reactants;/* Name of each reactant species. */
  StdStringList products;/* Name of each product species. */
  StdBool third_body;/* Whether the reaction occurs in precense of catalytical third-body (e.g. a surface). */
  StdDouble A;/* Preexponential factor in the rate constant. [FIXME define the unit. As formulated in the documentation of the CHEMKIN II file format, it depends on b and the reaction order... consider use a saner expression for the reaction constant for this entity.] */
  StdDouble b;/* Parameter in the rate constant, see entity description. */
  StdDouble Ea;/* Activation energy. */
  StdDouble A_low;/* Preexponential factor for the low-pressure limit.  Support fillvalues.  FIXME: define the unit. */
  StdDouble b_low;/* Value of b in the low-pressure limit.  Support fillvalues. */
  StdDouble Ea_low;/* Activation energy in the low-pressure limit.  Support fillvalues. */
  StdDoubleArray troe;/* Parameters a, T***, T* and T** when using Troe's formula to express pressure dependency.  The last parameter T** is optional. */
  StdStringList enhancement_species;/* Name of species in the buffer gas corresponding to the enhancement_factors. */
  StdDoubleArray enhancement_factors;/* Enhancement factors for describing dependency of the rate parameters on the buffer gas. */
  StdDoubleArray P_plog;/* Pressures defining the borders of the nplog pressure intervals for defining pressure dependency of the rate constant.  Should be increasing. */
  StdDoubleArray A_plog;/* Preexponential factors for pressure dependency of the rate constant.  FIXME: define the unit. */
  StdDouble b_plog;/* Values of b for pressure dependency of the rate constant. */
  StdDouble Ea_plog;/* Activation energy for pressure dependency of the rate constant. */
 private:
  class Private;
  Private *d;
};

SOFT_END_NAMESPACE

#endif // CHEMKIN_REACTION_ENTITY_H_DEF
