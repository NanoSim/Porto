/* Fluent UDF using id{73479d68-1a1c-494f-aebe-b24564e6b0aa}*/ 

#include "udf.h"

real min_cutoff = 0.000001;			/* Adjust to ensure stability */
real dp = 0.00001;					/* Grain diameter */
real Temp = 1173;					/* Adjust to experimental run */
real Gas_Const = 8.314; 
real k0 = 2.5;						/* Pre-exponential factor */
real EA = 23.999;					/* Activation energy */

DEFINE_HET_RXN_RATE(NiO_CH4,c,t,r,mw,yi,rr,rr_t)
{
	real Rrate, k, S0, X_CH4, MW_CH4, vol_frac_solid;
	Thread *gas_thread = THREAD_SUB_THREAD(t,0);
	Thread *solid_thread = THREAD_SUB_THREAD(t,1);

	vol_frac_solid = C_VOF(c,solid_thread);
	X_CH4 = yi[0][2];    /*   Check second index if order of species in gas is altered */
	MW_CH4 = mw[0][2];	 /*   Check second index if order of species in gas is altered */

	S0 = 6.0 * 0.2066 * vol_frac_solid / dp; 
	k = k0 * exp(-EA/(Gas_Const*Temp));
	Rrate = k * S0 * pow((C_R(c,gas_thread) * X_CH4 / MW_CH4 * 1000.0 ),0.6);  

	if (X_CH4 < min_cutoff) Rrate = 0;     

	if (vol_frac_solid < min_cutoff) Rrate = 0;		  

	*rr = Rrate / 1000.0;
}

/* Fluent UDF using id{3566face-8295-4140-aaf3-cfded6df666f}*/ 

#include "udf.h"

real min_cutoff = 0.000001;			/* Adjust to ensure stability */
real dp = 0.00001;					/* Grain diameter */
real Temp = 1173;					/* Adjust to experimental run */
real Gas_Const = 8.314; 
real k0 = 2.5;						/* Pre-exponential factor */
real EA = 0;					/* Activation energy */

DEFINE_HET_RXN_RATE(NiO_CH4,c,t,r,mw,yi,rr,rr_t)
{
	real Rrate, k, S0, X_CH4, MW_CH4, vol_frac_solid;
	Thread *gas_thread = THREAD_SUB_THREAD(t,0);
	Thread *solid_thread = THREAD_SUB_THREAD(t,1);

	vol_frac_solid = C_VOF(c,solid_thread);
	X_CH4 = yi[0][2];    /*   Check second index if order of species in gas is altered */
	MW_CH4 = mw[0][2];	 /*   Check second index if order of species in gas is altered */

	S0 = 6.0 * 0.2066 * vol_frac_solid / dp; 
	k = k0 * exp(-EA/(Gas_Const*Temp));
	Rrate = k * S0 * pow((C_R(c,gas_thread) * X_CH4 / MW_CH4 * 1000.0 ),0.6);  

	if (X_CH4 < min_cutoff) Rrate = 0;     

	if (vol_frac_solid < min_cutoff) Rrate = 0;		  

	*rr = Rrate / 1000.0;
}

