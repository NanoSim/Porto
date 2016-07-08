(function (h5){
    'use strict';

    h5.NAME = 'H5';
    h5.VERSION = '0.1-SNAPSHOT-1';
    h5["^doc"] = 'This module contains methods for the HDF5 storage';

    Driver = function (options) {
	this.uri = options.uri || './default.h5';
    }

    Driver.prototype.write = function (entity, callback) {
	var h5 = new H5();
	h5.create(this.uri);
	h5.addGroup("meta/");
	h5.addGroup("data/");
	h5.write("meta/name", entity.__name__);
	h5.write("meta/version", entity.__version__);
	var o = entity.get();
	for (var property in o) {
	    h5.write("data/" + property, o[property]);
	}
	h5.close();
	return true;
    }

    Driver.prototype.read = function(id) {
	print ("reading");
    }

    h5.driver = function() {
	return Driver.prototype.constructor;
    }

    return h5;
})(exports);
