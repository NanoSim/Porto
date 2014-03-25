Entity = require('porto.entity');
JSONStorage = require('porto.storage.jsonstorage').jsonStorage;

__main__ = function()
{
    var storage = JSONStorage();
    var particles = Entity.instance('particles', '0.0.1');
    
    particles.diameter = 100e-6;
    particles.density  = 1000;
    storage.commit(particles);
}
