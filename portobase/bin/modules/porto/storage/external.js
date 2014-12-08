(function (external){
    var driver = 'default',
	url = 'default',
	ext = null;
    
    external.connect = function(connectInfo){
	driver = connectInfo.driver;
	url    = connectInfo.url;

	var module = 'porto.storage.external.' + driver;
//	print (module);
	if (isValidModule(module)) {
	    ext = require(module);
//	    print (ext.name() + " [" + ext.version() + "]");
	} else {
	    throw ("Undefined driver");
	}

	return external;
    };

    external.using = function (entityName, entityVersion) {
	A = require('porto.factory.extentity').createEntity(entityName,
							   entityVersion);

	A.prototype.getFromSource = function(key) {
	    ext.open(url);
	    return ext.read(key);
	    ext.close();
	};
		
	return A;
    };
    
    external.open = function(){
	return ext.open(url);
    };

    external.close = function(){
	return ext.close();
    };
    
})(exports);
