(function (h5){
    'use strict';

    h5.NAME = 'H5';
    h5.VERSION = '0.1-SNAPSHOT-1';
    h5["^doc"] = 'This module contains methods for the HDF5 storage';

    Driver = function (options) {
	this.uri = options.uri || './default.h5';
    }

    Driver.prototype.write = function (entity) {
	print ("write");
    }

    Driver.prototype.read = function(id) {
	print ("reading");
    }
})(exports);
