#include "udf.h"

/* 
	TODO This is a preliminary template.

	The input provided to this are at the moment:

		@{soft.model.effectivenessFactor.functionalForm}
		@{soft.model.effectivenessFactor.parameters}

	Producing the code:

		real x = effectiveness_factor_functional_form_constant(0.1234);

	The same goes for:

		@{soft.model.effectiveReactionparameters.functionalForm}
		@{soft.model.effectiveReactionparameters.parameters}
		@{soft.model.effectiveReactionparameters.multiplyByParticleVolumeFraction}

	so that we for example can generate the code:

	if (@{soft.model.effectiveReactionparameters.multiplyByParticleVolumeFraction} == 1) { // There is a bug with storing bools across language barriers, so I use an int for now 
		...
	}

	Assumption: We can drop argumentCount.

*/

real effectiveness_factor_functional_form_constant(real p1) {
	return 0.0; // TODO Something here
}

real effectiveness_factor_functional_form_somethingelse(real p1, real p2, real p3) {
	return 0.0; // TODO Something here
}

real effective_reaction_parameters_functional_form_constant(real p1) {
	return 0.0; // TODO Something here
}

real effective_reaction_parameters_functional_form_somethingelse(real p1, real p2, real p3) {
	return 0.0; // TODO Something here
}


// TODO The name defined here should probably be something else
DEFINE_HET_RXN_RATE(NiO_CH4,c,t,r,mw,yi,rr,rr_t)
{

	// NOTE: There are some issue with storing and communicating bools across language barriers, so we use an integer for now: 
	if (@{soft.model.effectiveReactionparameters.multiplyByParticleVolumeFraction} == 1) {
		// TODO Something here
	}

	real x = effectiveness_factor_functional_form_@{soft.model.effectivenessFactor.functionalForm}(@{soft.model.effectivenessFactor.parameters})



	// ------------------------------------------------------
	// TODO The below code is from the previous generator. I've kept this in place just to give an example.

	real Rrate, k, S0, X_CH4, MW_CH4, vol_frac_solid;
	Thread *gas_thread = THREAD_SUB_THREAD(t,0);
	Thread *solid_thread = THREAD_SUB_THREAD(t,1);

	vol_frac_solid = C_VOF(c,solid_thread);
	X_CH4 = yi[0][2];    /*   Check second index if order of species in gas is altered */
	MW_CH4 = mw[0][2];	 /*   Check second index if order of species in gas is altered */

	S0 = @{soft.model.S0Eq}; 
	k = @{soft.model.kEq};
	Rrate = @{soft.model.RrateEq};  

	if (X_CH4 < min_cutoff) Rrate = 0;     

	if (vol_frac_solid < min_cutoff) Rrate = 0;		  

	*rr = Rrate / 1000.0;
}

