
instance    = require('soft.entity').instance;
Collection  = require('soft.collection').Collection;
jsonStorage = require('soft.storage.jsonstorage').jsonStorage;

var storage = jsonStorage();

var collection = new Collection( instance('collection', '1.0') );
collection.setName("MyFirst");

var my1 = instance('myfirst', '1.0');
var my2 = instance('myfirst', '1.0');
collection.registerEntity(my1, 'my1');
collection.registerEntity(my2, 'my2');
collection.registerRelationship('my1', 'my2', 'has-a');
storage.commit(collection.object());

print(collection.uuid());
