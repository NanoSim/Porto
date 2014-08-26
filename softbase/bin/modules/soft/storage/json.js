/*
 * store (entity) - Store the entity to disk

*/
(function(json){
    'use strict';

    json.NAME = 'Json';
    json.VERSION = '0.1-SNAPSHOT-1';
    json["^doc"] = 'This module contains method for the Json storage';

    json.Driver = function (options) {
	this.uri = options.uri || "default.json";
	this.path = options.path || "./";
    }
    
    json.Driver.prototype.store = function (entity) {
	var e = (isString(entity) ? JSON.stringify(JSON.parse(entity)) : JSON.stringify(entity));
	var path = this.path + uuidgen();
	writeFile(path, JSON.stringify(entity));
    }

    json.Driver.prototype.read = function (id) {
	var path = this.path + id;
	return JSON.parse(readFile(path));
    }

    return json;
})(exports);
