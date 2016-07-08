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

__main__ = function (args)
{
    try {
	var collectionId = args[1],
	    collection   = new Collection(collectionId);

	print (JSON.stringify(collection.get(), undefined, 4));
	var seedId       = collection.findInstance('seed').uid,
	    guessId      = collection.findInstance('guess').uid,
	    seed         = new Number(seedId),
	    guess        = new Number(guessId),
	    match        = ((seed.value - guess.value) == 0) ;
	print ("seed num: ", seed.value, " - guess num: ", guess.value);

	if (match) {
	    print ("nice one!");
	}
	else {
	    collection.removeEntity('guess');
	    collection.store();
	    print ("sorry, try again");
	}
	return (match ? 0 : 1);
    } catch (err) {
	print (err);
    }
    return 1;
}
