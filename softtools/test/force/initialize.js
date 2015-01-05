Entity      = require('soft.entity');
Collection  = require('soft.collection');
JSONStorage = require('soft.storage.jsonstorage').jsonStorage;

__main__ = function()
{
    var storage = JSONStorage();
    var particles = Entity.instance('particles', '0.0.1');
    
    particles.diameter = 100e-6;
    particles.density  = 1000;
    var id = particles.uuid;
    print('ParticleEntityId ' + id);

    var mycase = Entity.instance( 'collection', '1.0') );
    var collection = new Collection( mycase );
    print('CollectionId ' + collection.uuid());
						      
}
