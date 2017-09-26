# Use Case 1. Coupling of REMARC and Ansys FLUENT

In this usecase we demonstrate that data from atomic scale can be used
in the generation of a FLUENT UDF utilizing offline coupling with
Porto. This simple demo uses some important fundamental features of
Porto:

  1. The ability to work with entities and metadata independent of storage mechanisms.
  2. Make use of the Porto internal storage for storing both metadata and data.
  3. Work with Collections for building Domain Specific Relationships.
  4. Utilize custom external plugins for retrieving data from proprietary formats.
  5. Seamless mixing of compiled and scripted languages (C++/JavaScript).

## Prerequisite

In order to run this workflow, it is assumed that there already exists
DFT data coming from VASP simulations. This includes

  * Gas Phase Species (large static folder of data)
  * Thermo Data (needed as input for REMARC later)

We will demonstrate the use of a 'Reference' entity to simply store the
location of the Gas Phase Species Data. For the accompanying
thermo-properties we want to store the entire file in our database for
later use.

## Create an initial collection with DFT-data
The dft-prepare is a tool written in C++ that takes the Gas Phase
Species directory and a thermo-file as argument. It will first
instantiate a 'Collection' and then populate this with the 'Reference'
and 'File' entities. Then the collection is stored in the MongoDB
backend database.

  1. Create a collection
  2. Create a File-entity and populate it with the contents of thermo-data
  3. Create a Reference-entity and populate it with the gas phase folder info 
  4. Attach the File and Reference entity in the collection
  5. Store the collection using an internal storage (MongoDB)
  6. Return the identity (UUID) of the collection for further use.

```bash
$ cd porto/examples
$ dft-prepare dft/Fe2O3/ dft/thermo.dat 
cc3bc435-159c-4e96-b53f-1b97a526d5ce
```

dft-prepare is a utility that can be found in the folder
"porto/src/dft-prepare". It is a small C++ program that imports the
auto generated C++ files "reference" and "file" created from entities
with the same name. (The Porto entities resides in
porto/src/entities).  The classes "soft::Reference" and "soft::File"
instantiated and initialized. 

```c++
soft::Reference reference;
reference.uri          = dftPathInfo.absoluteFilePath().toStdString();
reference.created      = dftPathInfo.created().toString("dd-mm-yyyy").toStdString();
reference.owner        = dftPathInfo.owner().toStdString();
reference.lastModified = dftPathInfo.lastModified().toString("dd-mm-yyyy").toStdString();  
...

soft::File file;
file.filename          = dftBoundInfo.fileName().toStdString();
file.suffix            = dftBoundInfo.suffix().toStdString();
file.size              = dftBoundInfo.size();
file.data              = dataFromFile(dftBoundInfo.absoluteFilePath());	
...
```

A new soft::Collection is then instantiated and attached with reference and file.

```c++
soft::Collection collection;
collection.attachEntity("dftPath", &reference);
collection.attachEntity("dftBoundayFile", &file);
...
```

## Run the REMARC simulation
REMARC consist of a set of Python-scripts. REMARC requires an
interactive session that has been automated for this demo. We can show
that this interaction can be replaced with using Porto scripting and an
entity for REMARC Setup.

The REMARC-wrapper consist of three distinct parts.

  1. Porto Input
  2. REMARC Simulation
  3. Porto Output

The wrapper assumes that there exist a Collection with correctly
registered entities. (From dft-prepare).  The "Porto Input" fetches the
File and Reference entities from the collection.

The REMARC Simulation starts the Python scripts for a shell-process
with the path to the Gas Species directory as argument. When the
process has finished, a new Reference Entity is created - pointing to
the SurfaceChemkin.inp file - for later reference. Additional metadata
about the simulation can also be attached (version of the simulation
software, ++) for later use.

The Porto Output will prepare the CHEMKIN-II data format produced by
the REMARC-simulation. Here, the external storage backend 'chemkin' is
used to read CHEMKIN-II data into a set of Chemkin-Entities that will
be stored in the internal storage (mongodb). This data can be further
used to reproduce the original CHEMKIN-II data files if needed.

```bash
$ ./remarc-wrapper remarc/ {cc3bc435-159c-4e96-b53f-1b97a526d5ce}
bin size: == 1
started
Extracting VASP data from: /home/user/nanosim-demo/dft/Fe2O3
...
bin size: == 6845
Checking the format of the chem.inp file.
chem.inp file format check PASSED.
Parsing NASA thermo file: /HOME/USER/NANOSIM-DEMO/REMARC/THERMO.DAT
End of Parsing NASA thermo file: /HOME/USER/NANOSIM-DEMO/REMARC/THERMO.DAT
Global Units are NO GLOBAL UNITS
Data output to speciesParsed and reactionsParsed.

Parsing NASA thermo file: /HOME/USER/NANOSIM-DEMO/REMARC/THERMO.DAT
End of Parsing NASA thermo file: /HOME/USER/NANOSIM-DEMO/REMARC/THERMO.DAT
```

## Run the ANSYS Fluent UDF Code Generator
The code generator is a Porto-script that utilizes the 'soft.mvc' to
create a text based on a template-file and a data source. In our case
the template is based on an existing UDF. We will show that we can
generate (a set of) UDFs from reaction data stored in a database.

The utility genudf.js is found in (porto/examples/udfgen) and uses the
code generation facility of SOFT5. The entity
eu.nanosim.vasp/ChemkinReaction/0.1 is used to create ChemkinReaction
objects. In this example, we search the collection for a relationship:

```js
var reactionDataIds = collection.findRelations("reactiondata", "has-id");
```

This will return a list of ids for a set of ChemkinReaction
datapoints. We can then instantiate each one and populate it with
content from the database:

```js
reactionDataIds.forEach(function(reactionId){
			var reaction = new porto.ChemkinReaction(reactionId);
			reaction.read(storage);
...
```
	
In the SOFT5 MVC (Model-View-Controller) framework, the model
constitutes an object with properties that is passed to a view-template.

```js
var controller = require('soft.mvc').create({
	model: {
		min_cutoff: 1e-6,
		dp : 0.00001,
		k0 : 2.5,
		Temp: 1173.,
		EA: reaction.Ea, /* Read value from coming for the Porto database */
		Gas_Const: 8.314,
		S0Eq: "6.0 * 0.2066 * vol_frac_solid / dp", 
		kEq: "k0 * exp(-EA/(Gas_Const*Temp))",
		RrateEq: "k * S0 * pow((C_R(c,gas_thread) * X_CH4 / MW_CH4 * 1000.0 ),0.6)"
	},
	view: "./template/udf.cjs"
});
```

The template is a text file (udf.cjs) which contains markups where
expressions will be replaced by the evaluated value by a javascript
engine. In this way we can start with a final document as we want it
to be generated, and the replace all parameters with input data from
the model. (The same technique is also used for generating source
code).

```js
#include "udf.h"

real min_cutoff = @{soft.model.min_cutoff}; /* Adjust to ensure stability */
real dp = @{soft.model.dp};                 /* Grain diameter */
real Temp = @{soft.model.Temp};             /* Adjust to experimental run */
real Gas_Const = @{soft.model.Gas_Const};
real k0 = @{soft.model.k0};                 /* Pre-exponential factor */
real EA = @{soft.model.EA};                 /* Activation energy */
```

Here we see that the activation energy (soft.model.EA) comes from the
model.EA property defined earlier. This value comes from the previous
calculations. 

Running the genudf utility gives the following:

```bash
$ ./genudf.js {cc3bc435-159c-4e96-b53f-1b97a526d5ce}
```

This produces the two UDFs

```c++
/* Fluent UDF using id{fcf0db6f-3961-4f67-920a-8c5f4733994f}*/ 

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

/* Fluent UDF using id{688c0ccd-acc4-41f8-832f-d4c8112df049}*/ 

#include "udf.h"

real min_cutoff = 0.000001;			/* Adjust to ensure stability */
real dp = 0.00001;					/* Grain diameter */
real Temp = 1173;					/* Adjust to experimental run */
real Gas_Const = 8.314; 
real k0 = 2.5;                      /* Pre-exponential factor */
real EA = 0;                        /* Activation energy */

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
```

## Summary

In this use case we saw how a Collection was instanciated from a C++
code and how instances of other entities was attached to it. The
REMARC-wrapper used the generated collection as input ran the REMARC
(Python) code as an embedded process. The generated ChemkinII files
was then read and its contents written into the database and attached
to the collection. In the final step we saw how JavaScript could be
employed to search for registered entities within a collection and use
the code generator to generate source code.
