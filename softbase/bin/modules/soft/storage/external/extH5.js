(function (extH5){
    var _name = "Ext Storage HDF5",
	_version = "0.1-SNAPSHOT-1",
	_h5 = new H5();

    extH5.version = function() {
	return _version;
    }

    extH5.name = function() {
	return _name;
    }
    
    extH5.open = function(url) {
	return _h5.open(url);
    }

    extH5.close = function() {
	return _h5.close();
    }

    extH5.read = function (key) {
	var value = _h5.read(key);
	return value;
    }

})(exports);
