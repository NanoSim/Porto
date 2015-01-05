/*!
 jsonStorage is a superlightweight storage method that will just dump
 and entity to a .json-file, and read it back again as a json-object
*/
jsonStorage = function(){
    return {
	commit: function(obj) {
	    var json = JSON.stringify(obj);
	    writeFile(obj.__name__ + '_' + obj.uuid, json);
	    return true;
	},
	fetch: function(name, uuid) {
	    var json = readFile(name + '_' + uuid);
	    if(json === undefined) throw ("Cannot find entity in storage");
	    return JSON.parse(json);
	}
    };
};

exports = jsonStorage;

