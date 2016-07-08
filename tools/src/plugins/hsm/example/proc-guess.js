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
	var collectionId = args[1],
	    collection   = new Collection(collectionId),
	    rangeId      = collection.findInstance('range').uid,
	    range        = new Range(rangeId),
	    attempts     = collection.findInstance('attempts') == undefined ? new Number() : new Number(collection.findInstance('attempts').uid),
	    guess        = new Number(),
	    random       = range.min + Math.floor(Math.random()*(range.max-range.min));
	
	guess.value = random;
	if (attempts.value == undefined) {	    
	    attempts.value = 1
	    collection.registerEntity(attempts, 'attempts');
	} else {
	    attempts.value = attempts.value + 1;
	}
	guess.store();
	attempts.store();
	collection.registerEntity(guess, 'guess');
	collection.store();
	print ("num attempts: ", attempts.value);
	print (collection.id);
	
    } catch (err) {
	console.log(err);
    }
    return 0;
}
