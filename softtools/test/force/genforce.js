//Collection    = require('porto.collection').Collection;
//Entity        = require('porto.entity').Entity;
//Storage       = require('porto.storage.jsonstorage').jsonStorage;
Time          = require('porto.utils.time').Time;
ExtStorage    = require('porto.storage.external').ExternalStorage;
//createFromExt = require('porto.storage.external').createEntityFromExternalSource;

var time = new Time();
/* Set up an external datasource with the registered driver 'LogData' */
var storage       = ExtStorage.create({
    driver:"LogData",
    url: "file:///path/to/file"
});

print("elapsed: ", time.elapsed());
    

/* Read the contents of the file that matches the defined properties of logdata */
//var particleDist  = storage.fetch('logdata', '1.0');



