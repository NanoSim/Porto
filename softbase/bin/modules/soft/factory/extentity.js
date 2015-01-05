(function (extentityfactory){
    extentityfactory.createEntity = function(entityName, entityVersion, setterCallback)
    {
	var META_DATABASE = 'meta',
	    META_COLLECTION = 'entities';

	var mclient = MongoClient (),
	    coll = mclient.collection(META_DATABASE, META_COLLECTION),
	    cursor  = coll.find({
		name: entityName, 
		version: entityVersion
	    }),
	    bson = cursor.next();

	if (bson == undefined) {
	    var errormsg = '[ERROR] cannot find entity ' + entityName+':'+entityVersion+' in database';
	    console.error(errormsg);	    
	    return undefined;
	}

	var obj = JSON.parse(bson.asString());
	var def = "(function(){function b(){this.__name__='"+ entityName +
	    "'; this.__version__='" + entityVersion+"';";
	
	obj.properties.forEach (function (property){
	    if(property.dims != undefined) {
		def += "this."+property.name+"=[];";
	    }});

	def +=  "this.read();};" + 
	    "b.prototype.accept=function(v){return v.visit(this);};";


	obj.properties.forEach (function (property) {
	    var pname = property.name.charAt(0).toUpperCase() + property.name.slice(1),
		getter = "get" + pname;
	    def += "b.prototype." + getter + " = function() {return this." + property.name + ";};";	
	});
	def += "b.prototype.read = function(){";
	obj.properties.forEach (function (property) {
	    def += "isFunction(this.getFromSource) && (" +
		"this." + property.name + " = this.getFromSource('"+property.name+"'));";
	});
	def += "};"
	def += "b.prototype.get = function() {return {" + obj.properties.map (function (prop) {
	    return prop.name + ":" + "this." + prop.name;
	}).join(",") + "};};";
	
	def += "return b.prototype.constructor;})();";

	return eval(def);
    };
    
})(exports);
