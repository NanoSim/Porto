Entity    = require('porto.entity');
Collection  = require('porto.collection');
jsonStorage = require('porto.storage.jsonstorage').jsonStorage;

__main__ = function(args) 
{
    var storage = jsonStorage();
    var collection = new Collection( instance('forceModelCollection', '0.1') )
    collection.setName('ForceColl');

    var particles = instance('particles', '0.0.1');
    var coupling  = instance('coupling', '0.0.1');
    collection.registerEntity(particles, 'particles1');
    collection.registerEntity(coupling, 'coupling1');
    storage.commit(collection.object());

    var uuid = collection.uuid();
    print(uuid);
    return uuid;
}
