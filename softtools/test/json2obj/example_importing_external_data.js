instance    = require('soft.entity').instance;
Collection  = require('soft.collection').Collection;
jsonStorage = require('soft.storage.jsonstorage').jsonStorage;

var storage = jsonStorage();

var sourceInfo = {
    url: "file:///path/to/data",
    schema: "externaldata",
    schema_version: "1.0"
};

var destinationInfo = {
    schema: "myfirst",
    schema_version: "1.0"
}

/*! The importData is implementing a strategy pattern that will find
 *  the appropriate translator, instanciate the appropriate external
 *  data source drivers, and fill the destination with data.
*/
var my1 = importData(sourceInfo, destinationInfo);
storage.commit(my1);
