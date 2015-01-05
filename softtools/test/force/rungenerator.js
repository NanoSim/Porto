include('forcemodelgenerator.js')

Storage    = require('soft.storage.jsonstorage').jsonStorage;
Collection = require('soft.collection');

generateForceModel = function(collectionId, storage) 
{
    var collection = new Collection( storage.fetch('forceModelCollection', collectionId) );

    var generator = new ForceModelGenerator(collection.obj);
    print(generator.generateForceModelSource());
    print(generator.generateForceModelHeader());
}


__main__ = function(args) 
{
    var storage = jsonStorage();
    args.slice(1).map(function(collectionId) {
	generateForceModel(collectionId, storage);
    });
}		      
		      
