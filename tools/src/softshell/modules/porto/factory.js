function quote( str ) {
    return str.replace(/\n/g,'\\n').replace(/\"/g,'\\\"').replace(/\'/g,'\\\'')
}

(function (entityfactory){   
    var META_DATABASE = 'meta',
        META_COLLECTION = 'entities';

    entityfactory.createEntity = function(entityName, entityNamespace, entityVersion, callback) {
	var mclient = MongoClient();
	var coll    = mclient.collection(META_DATABASE, META_COLLECTION);
	var cursor  = coll.find({
	    name: entityName,
	    version: entityVersion
	});
	
	var bson = cursor.next();
	if (bson == undefined) {
	    var errormsg = '[ERROR] cannot find entity ' + entityName+':'+entityVersion+' in database';
	    if (isFunction(callback)) {
		callback(errormsg);		
	    } else {
		console.error(errormsg);
	    }
	    return undefined;
	}

        
	var obj = JSON.parse(bson.asString());
        obj["_id"] = undefined;
        var schema = JSON.stringify(obj);
        var def = "(function(){function b(arg){";
        
        obj.properties.forEach (function (property){
	    if(property.dims != undefined) {
		def += "this."+property.name+"=[];";
	    }});

        def += "this.create(arg);";
        def += "this.setSchema(\"" + quote(schema)+ "\");";

        obj.properties.forEach (function (property) {
	    var pname = property.name.charAt(0).toUpperCase() + property.name.slice(1),
		setter = "set" + pname,
		getter = "get" + pname;
	    def += "b.prototype." + setter + " = function(a) {this." + property.name + "=a;};" +
		"b.prototype." + getter + " = function() {return this." + property.name + ";};";	
	});

        var sets = [];
        var gets = [];
        obj.properties.forEach (function (property) {
            var setline = "this.generic.setProperty('"+property.name+"', this."+property.name+");";
            var getline = "this."+property.name+" = this.generic.property('"+property.name+"');";
            sets.push(setline);
            gets.push(getline);
        });

        def += "b.prototype.setAll=function(){"+sets.join("")+"};";
        def += "b.prototype.getAll=function(){"+gets.join("")+"};";
        def += "};return b.prototype.constructor;})();";
	
        return eval(def);
        

    };
    return entityfactory;
})(exports);
