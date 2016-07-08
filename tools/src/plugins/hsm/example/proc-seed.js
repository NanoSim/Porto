#!/usr/bin/env softshell

var driverInfo = {
    driver: 'mongodb',
    uri: 'mongodb://localhost',
    database: 'demo',
    collection: 'guess'
};

var entity = require ('soft.entity').db(driverInfo);
Collection = require ('soft.collection').db(driverInfo);
Number     = entity.using ('Number', '0.1');
Range      = entity.using ('Range', '0.1');

__main__ = function (args)
{
    try {
	var seed = new Number();
	var range = new Range();
	var min = 0,
	    max = 10,
	    value = min + Math.floor(Math.random()*(max-min));
	
	range.set({
	    min: min,
	    max: max,
	});
	seed.value = value;
	range.store();
	seed.store();
	
	var coll = new Collection();
	coll.registerEntity(range, 'range');
	coll.registerEntity(seed, 'seed');	
	coll.store();
	print (coll.id);
    } catch (err) {
	console.log (err);
    }

    return 0;
}
