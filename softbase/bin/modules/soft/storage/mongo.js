(function(mongo){
    'use strict';

    mongo.NAME = 'Mongo';
    mongo.VERSION = '0.1-SNAPSHOT-1';
    mongo["^doc"] = 'This module contains method for the MongoDb storage';

    Driver = function(options) {
	this.uri        = options.uri || "mongodb://localhost";
	this.database   = options.database || "soft";
	this.collection = options.collection || "default";

	this.mongoclient = new MongoClient(this.uri);
	this.mongocollection = this.mongoclient.collection (this.database,this.collection);
    }

    Driver.prototype.find = function(query) {
	var cursor = this.mongocollection.find(query);
	var ret = [];
	var bson = cursor.next();
	while(bson != undefined) {
	    var obj = JSON.parse(bson.asString());
	    ret.push(obj); 
	    bson = cursor.next();
	}
	return ret;
    }

    Driver.prototype.selfTest = function () {
	var tests = [];
	
	function addTest(testName, fn) {
	    tests.push({
		name: testName,
		result: fn()
	    });
	}
	
	function report() {
	    tests.forEach(function(test){
		print(test.name, ':', test.result);
	    });
	}
	
	addTest('MongoClient_isRegistered', function(){
	    return (typeof MongoClient !== 'undefined');
	});

	addTest('MongoClient_databaseNames', function(){
	    var mongoc = new MongoClient();
	    var names = mongoc.databaseNames();
	    return false;
	});

	report();
    }

    Driver.prototype.write = function (entity, callback) {
	/*
	var num = this.mongocollection.count({name: entity.__name__, version: entity.__version__});
	if (num > 0) {
	    console.error("entity already exists in database. exiting");
	    return;
	}
	*/
	var result = this.mongocollection.insert(entity);
	if (isFunction(callback)) {
	    return callback(result);
	}
	return result;
    }

    Driver.prototype.read = function (uuid, callback) {
	var cursor = this.mongocollection.find({id: uuid}); 
	var bson = cursor.next();
	if (isFunction(callback)) {
	    return callback(bson);
	}
	return bson;
    }

    mongo.driver = function() {
	return Driver.prototype.constructor;
    }

    return mongo;
})(exports);
