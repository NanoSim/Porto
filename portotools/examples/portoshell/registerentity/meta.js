(function(meta){
    'use strict';

    function Container() {
	var client = new MongoClient("mongodb://localhost/");
	this.collection = client.collection("meta", "entities");
    }
    
    Container.prototype.store = function(entity) {	
	var e = (isString (entity) ? 
		 JSON.stringify (JSON.parse (entity)) : 
		 JSON.stringify (entity));
	print (e);
	return this.collection.insert(e);
    };

    Container.prototype.find = function(entityName, entityVersion) {
	var query = {
	    name: entityName,
	    version: entityVersion
	};
	
	print(JSON.stringify(query));
	var cursor = this.collection.find(JSON.stringify(query));
	var bson = cursor.next();
	return bson.asString();
    };

    meta.connect = function() {
	var c = new Container();
	return c;
    };

    return meta;
})(exports);
