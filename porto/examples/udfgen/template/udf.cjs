#include "udf.h"

real min_cutoff = @{soft.model.min_cutoff};			/* Adjust to ensure stability */
real dp = @{soft.model.dp};					/* Grain diameter */
real Temp = @{soft.model.Temp};					/* Adjust to experimental run */
real Gas_Const = @{soft.model.Gas_Const}; 
real k0 = @{soft.model.k0};						/* Pre-exponential factor */
real EA = @{soft.model.EA};					/* Activation energy */

DEFINE_HET_RXN_RATE(NiO_CH4,c,t,r,mw,yi,rr,rr_t)
{
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

