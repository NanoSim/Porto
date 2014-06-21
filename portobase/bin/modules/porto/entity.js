(function (entity){
    function Entity(driverInfo)
    {
	var storage = require('porto.storage').connect(driverInfo);    	
	
	this.using = function (entityName, entityVersion) {
	    A = require('porto.factory.entity').createEntity(entityName, entityVersion,
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
			self.set(JSON.parse(bson.asString()));
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
