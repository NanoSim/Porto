(function (entityfactory){
   
    var META_DATABASE = 'meta',
        META_COLLECTION = 'entities';

    entityfactory.createEntity = function(entityName, entityVersion, callback) {
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

	var def = "(function(){function b(arg){" +
	    "this.__name__='" + entityName +"';" +
	    "this.__version__='"+ entityVersion +"';";
	obj.properties.forEach (function (property){
	    if(property.dims != undefined) {
		def += "this."+property.name+"=[];";
	    }});

	def += "if (Object.prototype.toString.call(arg) === '[object String]') {"+
	    "this.id = arg;isFunction(this.read) && this.read();}else {this.id = uuidgen().substr(1, 36);"+
	    "if (Object.prototype.toString.call(arg) === '[object Object]') {"+
	    "if (arg.__name__ !== this.__name__ || arg.__version__ !== this.__version__) {"+
	    "isFunction(this.translate) && this.translate(arg);}}}};";
	

	def += "b.prototype.accept=function(v){return v.visit(this);};";	
	obj.properties.forEach (function (property) {
	    var pname = property.name.charAt(0).toUpperCase() + property.name.slice(1),
		setter = "set" + pname,
		getter = "get" + pname;
	    def += "b.prototype." + setter + " = function(a) {this." + property.name + "=a;};" +
		"b.prototype." + getter + " = function() {return this." + property.name + ";};";	
	});
	
	def += "b.prototype.set = function(obj) {" + obj.properties.map (function (prop) {
	    return "this." + prop.name + "=obj." + prop.name + ";";
	}).join("") + "};";    
	def += "b.prototype.get = function() {return {" + obj.properties.map (function (prop) {
	    return prop.name + ":" + "this." + prop.name;
	}).join(",") + "};};";
	def += "b.prototype.translate = function(a){require('soft.translator').translate(a,this);};";
	def += "return b.prototype.constructor;})();";

	if (isFunction(callback)){
	    callback(undefined, def);
	}

	return eval(def);
    };


    return entityfactory;
})(exports);

