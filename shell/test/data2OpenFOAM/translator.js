/*

The visitor pattern applied on entities can be applied in many
situations

 1. Default value initialization
 2. Translation between entity versions
 3. Importing from external data source to entities

*/

/*
 * Entity class
 */
function Entity(obj) {    
    this.obj = obj;
}

Entity.prototype.accept = function(visitor) {
    return visitor.visit(this);
};


function TranslatorVisitor() {
    var that = this;
}

TranslatorVisitor.prototype.visit = function(entity) {
    var tranObj = {
	a: entity.obj.b * 0.5,
	b: entity.obj.c.map( function(v){return v*2;} )
    };
    return tranObj;
};


var translator = new TranslatorVisitor();

var sourceObj = {
    b: 10.0,
    c: [1,2,3]    
};
var sourceEntity = new Entity(sourceObj);
var translated = sourceEntity.accept(translator);
print (JSON.stringify(translated));

var trans = new TranslatorFactory(source.schema, dest.schema);
