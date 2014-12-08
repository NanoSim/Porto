(function (jsonstore) {
    'use strict';

    var client = null,
        collection = null;

    jsonstore.connect = function(uri) {
	client = new MongoClient(uri);
    }
    
    jsonstore.setDatabase = function(database, coll) {
	collection = client.collection(database, coll);
    }

    jsonstore.store = function(json) {
	var e = (isString (json) ? 
		 JSON.stringify (JSON.parse (json)) : 
		 JSON.stringify (json));
	return collection.insert(e);	
    }

})(exports);
