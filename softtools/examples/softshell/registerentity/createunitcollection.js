/*
 *  This is an example script 
 */
var unitsJSON = fs.readFile('units.json', function(err, data){
    if (err) throw(err);

    /* connectivity information */
    var driverInfo = {
	driver:     'mongodb',
	database:   'soft',
	collection: 'units'
    };

    /* Entity and Collection creators */
    Collection = require('soft.collection').db(driverInfo);
    var entity = require('soft.entity').db(driverInfo);   
    Unit       = entity.using('unit', '1.0-SNAPSHOT-1');

    /* Parse the external file into a javascript object */
    var obj = JSON.parse(data);

    /* Instanciate a new Collection class */
    var unitLibrary = new Collection();
    unitLibrary.setName('UnitCollection');
    unitLibrary.setVersion('1.0');
    
    obj.units != undefined && obj.units.forEach (function(unitObj) {
	/* Create a new Entity for each element in the list */
	var unitEntity = new Unit();
	unitEntity.set(unitObj);
	unitEntity.store();

	/* Register the entity in the Collection class*/
	unitLibrary.registerEntity (unitEntity, 'entity'+unitEntity.code);
    });

    /* Store the Collection */
    unitLibrary.store();

    /* Display the id of the collection */
    console.log (unitLibrary.id);
});
