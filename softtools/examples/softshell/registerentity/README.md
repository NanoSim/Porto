
# Register metadata.


softshell can run a script with arguments given it contains a function:

	__main__ = function (args) {
		...
	}

To call this method from the command shell, you can simply type

	$ softshell myscript.js argument1 argument2

The method 'register.js' takes meta-entity declarations as arguments
and uploads these into the meta-database in mongodb. An example of
this is the definition of the unit-entity:


	{
		"name": "unit",
		"version": "1.0-SNAPSHOT-1",
		"description": "Unit definitions",
		"properties": [
			{
				"name": "abbreviation",
				"type": "string"
			},
			{
				"name": "code",
				"type": "string"
			},
			{
				"name": "conversionMultiplier",
				"type": "double"
			},
			{
				"name": "conversionOffset",
				"type": "double"
			},
			{
				"name": "quantityKind",
				"type": "string"
			},
			{
				"name": "symbol",
				"type": "string"
			},
			{
				"name": "unit",
				"type": "string"
			}
		]
	}



The name of this file is 'unit.json'. To insert this file into the
meta-database, the procedure is:

	$ softshell register.js units.json

	2014/06/14 15:00:40.0110: [18265]: INFO: cluster: Client
	initialized in direct mode.
	{"name":"unit","version":"1.0-SNAPSHOT-1","description":"Unit		
	definitions","properties":[{"name":"abbreviation","type":"string"},
	{"name":"code","type":"string"},{"name":"conversionMultiplier",
	"type":"double"},{"name":"conversionOffset","type":"double"},{"name":
	"quantityKind","type":"string"},{"name":"symbol","type":"string"},
	{"name":"unit","type":"string"}]}

#  Creating instances of entites

Creating an entity of type unit can now be performed runtime in the
scripting environment:

	Unit = require('soft.factory.entity').createEntity('unit', '1.0-SNAPSHOT-1');
	var u = new Unit();

The variable 'u' is now holding a new instance of the class 'Unit'. It
is also possible to call createEntity with a callback function that
will capture errors and the class definition (the function that
creates the class passed as text).

	Unit = require('soft.factory.entity').createEntity('unit', '1.0-SNAPSHOT-1',
		function(err, source){
			if (err) throw (err);
			console.log (source);
			});

# Storing data to a database

The storage where we want to store the entity values could be a number
of different locations. The storage-module contains a factory pattern
that allows us to specify the name of the driver we want to use,
together with other info.

	var storage = require('soft.storage').connect({
        driver: "mongodb",
        uri: "mongodb://localhost",
        database: "soft",
        collection: "mydata"
    });

In this case we want to store our data in a database called 'soft',
in a collection we call 'mydata'.

Let's fill an entity with some data and store the contents to the
database.

	var unit = new Unit();
	u.set({
        abbreviation: "Gy/s",
        code: "0780",
        conversionMultiplier: "1.0e0",
        conversionOffset: "0.0",
        quantityKind: "quantity:AbsorbedDoseRate",
        symbol: "Gy/s",
        unit: "GrayPerSecond"
	});

The next step is to store this in our selected storage target.

	storage.write(unit);

# Reading data back from a database

Our storage device have defined both read and write, so we can reuse
our storage from the previous example. To read back data we can call
the following method

	storage.read(id, [callback(bson)]);

If read is called with a callback function, the return value of the
callback with be the return value from the read-function. Otherwise
the read function will just return the Bson object.

	storage.read(myId, function(bson) {
	    var u = new Unit(myId);
		u.set(JSON.parse(bson.asString()));
		return u;
	});

This function will explicitly build a new Unit-entity and fill its
contents with the data from the database.


# Simplify interfaces by hiding the details

By hiding all details about storage and entity factories, and just
exposing the entity class, we can simplify working with the data:

	/* Connect the entity-instance generator to a datasource */
	var entitydb = require('soft.entity').db({
        driver: "mongodb",
        database: "newdb",
        collection: "newcoll"
    });

	/* Generate constructors for registered entities */
	Unit = entitydb.using('unit', '1.0-SNAPSHOT-1');

	var u = new Unit();
	u.set({
        abbreviation:         "Gy/s",
        code:                 "0780",
        conversionMultiplier: "1.0e0",
        conversionOffset:     "0.0",
        quantityKind:         "quantity:AbsorbedDoseRate",
        symbol:               "Gy/s",
        unit:                 "GrayPerSecond"
    });

    u.store();

	var v = new Unit(u.id);
	print (v.symbol);

In this example we simply instanciate a Unit class, set some values,
and call store(). To retrieve data from the database we simply use the
identifier of the entity.



