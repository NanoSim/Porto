/*
  This is utility module that hardcodes the use of MongoDB for storing meta-data
*/

(function(metastore){
    'use strict';

    function Container(connectInfo) {
	var client = new MongoClient(connectInfo.uri);
	this.collection = client.collection(connectInfo.database, connectInfo.collection);
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
    
    metastore.connect = function(connectInfo) {
	var c = new Container(connectInfo);
	return c;
    };
    
    return metastore;
    
})(exports);
