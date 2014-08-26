/*!
 * This is an example of how to instanciate an entity from an entity metafile
 ***/


// Fetch the instance-function from the soft.entity module
instance    = require('soft.entity').instance;


// Create an instance of entity 'Example' version 1.0
var example = instance('example', '1.0');

// Verify that the instance has a unique ID
print(example.uuid);

// Show the contents of the object
print(JSON.stringify(example));
