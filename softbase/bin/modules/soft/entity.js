(function (entity){
    function Entity(driverInfo)
    {
	var storage = require('soft.storage').connect(driverInfo);    	
	
	this.using = function (entityName, entityVersion) {
	    A = require('soft.factory.entity').createEntity(entityName, entityVersion,
							    function(err)
							    {
								if (err) throw (err);
							    });
	    
	    A.prototype.store = function(){
		storage.write(this);
	    };
	    
	    A.prototype.read = function(){

		var self = this;
		storage.read(this.id, function(bson){
		    if (isQObject(bson)) {

			var obj = JSON.parse(bson.asString());
			if (obj.__name__ !== self.__name__ || obj.__version__ !== self.__version__) {
			    self.translate(obj);
			}
			else {
			    self.set(obj);
			}
 		    }
		});
	    };
	    
	    return A.prototype.constructor;
	};
    };

    entity.db = function(driverInfo) {
	return new Entity(driverInfo);
    };
})(exports);
