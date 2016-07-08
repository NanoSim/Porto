(function(storage){
    'use strict';

    storage.NAME    = 'Storage';
    storage.VERSION = '0.1-SNAPSHOT-1';
    storage["^doc"] = 'This module contains methods for Internal Storage';

    function GenericStorageDriver (options) 
    {
	this.uri = options.uri || "default";
    }

    // Define the factory class
    function StorageDriverFactory() {}
   
    // Factory method for creating new storage driver instances
    StorageDriverFactory.prototype.createStorageDriver = function (options) {
	print("StorageDriverFactory.prototype.createStorageDriver");
	switch (options.driver) {
	case "mongo":
	case "mongodb":
	    MongoDriver = require('soft.storage.mongo').driver();
print("Creating MongoDriver: ", options.uri);
	    return new MongoDriver(options);
	    break;
	case "json":
	    var JsonDriver = require('soft.storage.json').Driver;
	    return new JsonDriver (options);
	    break;
	case "hdf5":
	case "h5":
	    var H5Driver = require('soft.storage.hdf5').driver();
	    return new H5Driver (options);
	    break;
	default:
	    throw('[UNKNOWN] Unknown driver type: ' + options.driver);
	}
	return undefined;
    }
    
    // Connect to a storage driver using the StorageDriverFactory
    storage.connect = function(options){
	if (typeof options === 'undefined') {
	    throw ('[UNDEFINED] Undefined argument to storage.connect');
	}

	if (typeof options.driver === 'undefined') {
	    throw ('[UNDEFINED] Undefined driver arguments');
	}

	var driverFactory = new StorageDriverFactory();
	var storageDriver = driverFactory.createStorageDriver (options);
	return storageDriver;
    }

    storage.connect['^doc'] = 'Connecting to a storage driver';    

    return storage;
})(exports);
