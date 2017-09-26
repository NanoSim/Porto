
# Use Case 2. Coupling of REMARC to parScale

In this example we demonstrate how to use data from an atomic scale simulation in **REMARC** as input to the particle scale model **parScale**, using **Porto**.

## Walkthrough

We assume that a simulation in REMARC has already been run and the simulation results have been stored in a *Porto Collection* with a unique identifier (*uuid*). This is described in the section _Run the REMARC simulation_. The uuid allows us to access the contents of the collection across different software and simulation tools.

Before we do the actual connection, we run the tool `inspectChemkinReactionEntity.js` which takes an uuid of a collection as argument. This allows us to inspect the contents of the Collection. In this example walkthrough we do this visual inspection in order to verify that the data is in place and that we are using the correct uuid. The following output is produced:

```bash
$ ./inspectChemkinReactionEntity.js a6a71841-139a-4310-a9e6-ef7a6f161a6f
Collection (uuid = a6a71841-139a-4310-a9e6-ef7a6f161a6f)
|    Reactants => Products, A, b, Ea
+--- CH4(S1) => CH4, 615900000000000000000, -2.54, 23.999
+--- CH4 => CH4(S1), 7553000000, -0.5, 0
```

We note that, for this example, the Collection contains two reactions, including reactants and its products alongside key information about these reactions (_A_, _b_ and _Ea_). This is the _output_ of the REMARC simulation, and will be _input_ to the parScale simulation.

After we have inspected this collection and are satisfied with its contents, we can run the script that does the actual connection: `chemkinReactionEntity-to-chemkinFile.js`. Running this script with the same uuid as input gives us the following output:

```bash
$ ./chemkinReactionEntity-to-chemkinFile.js a6a71841-139a-4310-a9e6-ef7a6f161a6f > Chemkin.inp
$ cat Chemkin.inp
ELEMENTS H C 
END
SPECIES
CH4(S1) CH4
END

REACTIONS KJOULES/MOLE MOLES
CH4(S1) => CH4 615900000000000000000 -2.54 23.999
CH4 => CH4(S1) 7553000000 -0.5 0
END 
```

We here see that the CHEMKIN II file has been populated with the reactions and their parameters, as inspected in the collection. This is now a file that contains the input needed to run parScale.

## Details

The source for these scripts can be found in the Porto repository under `porto/examples`. These scripts are meant to serve as examples of a range of techniques for connecting different software and can easilly be tailored towards other software, file types, data sets, entities and collections.

### `inspectChemkinReactionEntity.js`

The following code snippet describes how to use Porto to access and use data stored in collections. For this particular case, the reactions are printed to console.

```js
...

// Access the mongoDB database
var storage = new porto.Storage("mongo2", "mongodb://localhost", "db=porto;coll=demo");

// Load the collection given by the uuid on the command line, then retrieve all
// reaction entities in this collection
var uuid = args[1];
collection = new porto.Collection(uuid);
storage.load(collection);

print("Collection (uuid = " + uuid + ")"); 

// Find all the reactions attached to this collection
var reactionDataIds = collection.findRelations("reactiondata", "has-id");
print("|    Reactants => Products, A, b, Ea");

reactionDataIds.forEach(function (reactionId) {
   // Read each ChemkinReaction entity in the collection
   var reaction = new porto.ChemkinReaction(reactionId);
   reaction.read(storage);
   // Print out the details
   print("+--- " + reaction["reactants"] + " => " 
	+ reaction["products"] + ", " + reaction["A"] 
	+ ", " + reaction["b"] + ", " + reaction["Ea"]);
});

...
```


### `chemkinReactionEntity-to-chemkinFile.js`

`chemkinReactionEntity-to-chemkinFile.js` automates the following steps:

1. Access the Porto collection, referenced by the *uuid*.
2. Retrieve all reactions stored in the collection. Each reaction has the following information:
   * A list of reactants
   * A list of products
   * Pre-exponential factor _A_
   * Temperature exponent _b_
   * Activation energy, _Ea_
3. From the list of reactants and products, generate the list of _elements_ and _species_ involved in the reactions.
4. Use the reaction along with the generated list of elements and species to populate a *CHEMKIN II* template. 
5. Store the filled template to a file.

The way this is implemented in `chemkinReactionEntity-to-chemkinFile.js` is by first accessing the collection and retrieve its information:

```js
// Attempt to talk to the local mongodb
var storage = new porto.Storage("mongo2", "mongodb://localhost", "db=porto;coll=demo");

// Load the collection given by the uuid on the command line, then retrieve all
// reaction entities in this collection
var uuid = args[1];
collection = new porto.Collection(uuid);
storage.load(collection);

var reactionDataIds = collection.findRelations("reactiondata", "has-id");
```

The list of reactants and products, species and elements are then generated:

```js
// Collects all species, for example H2O, FeO3, CH4, etc.
var species = [];
// Collects all reaction lines, on the form 
// <reactants> => <product> <pre.exp.factor> <b> <activation energy>
var all_reactions = []
reactionDataIds.forEach(function (reactionId) {
   var reaction = new porto.ChemkinReaction(reactionId);
   reaction.read(storage);

   var reactants = reaction.reactants;
   var products = reaction.products;
      
   // Collect a list of all reactants and product species
   for (var r in reactants) {
      species.push(reactants[r]);
   }
   for (var p in products) {
      species.push(products[p]);
   }

   // Construct the reactions
   if (products.length > 0 && reactants.length > 0) {
      all_reactions.push(reactants.join(" + ") + " => " 
		  + products.join(" + ") + " " + reaction.A + " " 
		  + reaction.b + " " + reaction.Ea);
   }
});

// All elements we want to check for, ideally this could be the entire periodic table
var all_elements = ["Fe", "H", "C", "O"];
var elements = [];

unique(species).forEach(function(s) {
   all_elements.forEach(function(e) {
      // For each unique species, loop over each element, then ...
      var ss = s;
      // ... check if the element is in the species.
      if (find(e, ss)) {
         // We found an element, push it to the total list of elements and
         // strip it away from the string we are searching in.
         elements.push(e);
         ss = ss.replace(e, "");
      }
   });
});
```

Using this information, the CHEMKIN II file template can be populated:

```js
var controller = require('soft.mvc').create({
   model: {
      elements: unique(elements).join(" "),
      species: unique(species).join(" "),
      reactions: all_reactions.join("\n")
   },
   view: "./template/chemkin.cjs"
});
```

This template used here, `chemkin.cjs`, contains tags that corresponds to the names used in the above script which will substitute them for the actual values. For example, `@{soft.model.elements}` will here be replaced by a list of elements created above:

```
ELEMENTS @{soft.model.elements}
END
SPECIES
@{soft.model.species}
END

REACTIONS KJOULES/MOLE MOLES
@{soft.model.reactions}
END
```

### CHEMKIN II file format

For this scenario, we use a CHEMKIN II file with following simple structure:

```
ELEMENTS C H FE O 
END
SPECIES
CH4 H2 CO2 H2O FE2O3 FE3O4
END 

REACTIONS KJOULES/MOLE MOLES
   CH4 + 3FE2O3 => CO + 2H2 + 2FE3O4    1.0e+14 0.0 100.0 
   CH4 + 12FE2O3 => CO2 + 2H2O + 8FE3O4 1.0e+18 0.0 180.0
END 
```

The input file consists of three sections, each starting with a name and ending with the keyword `END`.

* `ELEMENTS` contains a list of elements noted by their periodic table names
* `SPECIES` contains a list of all species used in the reaction equations
* `REACTIONS` lists the possible reactions, followed by the pre exponential factor _A_, temperature exponent _b_ and activation energy _Ea_.

