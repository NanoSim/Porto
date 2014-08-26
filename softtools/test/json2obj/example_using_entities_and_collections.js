///////////// Example application

Collection  = require('soft.collection').Collection;
jsonStorage = require('soft.storage.jsonstorage').jsonStorage;
instance    = require('soft.entity').instance;


// Create a simple JSON-based storage class that will be used as database
var storage = jsonStorage();

// Instanciate an entity and store the object
var example = instance('example', '1.0');
example.len = 42;
storage.commit(example);
var uuid = example.uuid;

// Read back the entity into a copy-instance
var copy = storage.fetch('example', uuid);

// Test the collection class
var collection = new Collection( storage.fetch('collection', '57395dc8-c0be-4475-a942-8f28011f670a') );
if( collection.uuid() !== '57395dc8-c0be-4475-a942-8f28011f670a') throw ("uuid is wrong!");
if( collection.version() !== '1.0') throw ("Wrong collection version!");

// Create a brand new collection
var coll = new Collection( instance('collection', '1.0') );
coll.setName("NewCollection");
coll.setParent(collection);
storage.commit(coll.object());
