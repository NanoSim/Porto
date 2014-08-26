//Collection    = require('soft.collection').Collection;
//Entity        = require('soft.entity').Entity;
//Storage       = require('soft.storage.jsonstorage').jsonStorage;
Time          = require('soft.utils.time').Time;
ExtStorage    = require('soft.storage.external').ExternalStorage;
//createFromExt = require('soft.storage.external').createEntityFromExternalSource;

var time = new Time();
/* Set up an external datasource with the registered driver 'LogData' */
var storage       = ExtStorage.create({
    driver:"LogData",
    url: "file:///path/to/file"
});

print("elapsed: ", time.elapsed());
    

/* Read the contents of the file that matches the defined properties of logdata */
//var particleDist  = storage.fetch('logdata', '1.0');



