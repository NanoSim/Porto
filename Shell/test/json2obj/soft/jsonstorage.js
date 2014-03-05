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
}

