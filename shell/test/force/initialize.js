Entity = require('porto.entity');
JSONStorage = require('porto.storage.jsonstorage').jsonStorage;

__main__ = function()
{
    var storage = JSONStorage();
    var particles = Entity.instance('particles', '0.0.1');

}
