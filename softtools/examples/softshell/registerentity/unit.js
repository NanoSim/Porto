(function (){
    var storage = require('soft.storage').connect({
	driver: "mongodb",
	uri: "mongodb://localhost",
	database: "soft5",
	collection: "units"
    });    

    Unit = require('soft.factory.entity').createEntity('unit', '1.0-SNAPSHOT-1',
						       function(err)
						       {
							   if (err) throw (err);
						       });
    
    Unit.prototype.store = function(){
	storage.write(this);
    };

    Unit.prototype.read = function(){
	var self = this;
	storage.read(this.id, function(bson){
	    self.set(JSON.parse(bson.asString()));
	});
    };
})();
