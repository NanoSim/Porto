(function (entity){
    var generic = {}; // new soft.Entity();
    entity.using = function (entityName, entityNamespace, entityVersion) {
	A = require('porto.factory')
            .createEntity(entityName,
                          entityNamespace,
                          entityVersion,
			  function(err)
			  {
			      if (err) throw (err);
			  });       
        A.prototype.generic = generic;
        
        
        A.prototype.setSchema = function(arg){
            this.generic.setSchema(arg);
        };
        
        A.prototype.create = function(arg){
            if (typeof arg !== "undefined") {
                this.generic = new soft.Entity(arg);
            } else {
                this.generic = new soft.Entity();
            }
        };

        A.prototype.id = function(){
            return this.generic.id();
        }
        
	A.prototype.save = function(storage){
            this.setAll();
	    storage.save(this.generic);
	};
	
	A.prototype.read = function(storage){            
            storage.load(this.generic);
            this.getAll();
	};
	
	return A.prototype.constructor;
    };                
    
    return entity;
})(exports);
