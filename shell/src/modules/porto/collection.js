/*! The Collection class is a utility for encaptulating a collection
 *  object and providing a simple API for working on collections 
 *
 *  Collection are the definitions of sets of entities (including
 *  other collections), by defining parent collections, entities,
 *  relationships and dimensionality maps.
*/

function Collection(collObj) {    
    this.obj = collObj;
    if( this.obj.__version__    !== '1.0')     throw ("Illegal collection version");
    if( this.obj.dimensions     === undefined) throw ("undefined dimensions array in collection");
    if( this.obj.entities       === undefined) throw ("undefined entities array in collection");
    if( this.obj.relationships  === undefined) throw ("undefined relationships array in collection");
    if( this.obj.dimensionMap   === undefined) throw ("undefined dimension-map array in collection");
}

Collection.prototype.object = function() {
    return this.obj;
}

Collection.prototype.uuid = function() {
    return this.obj.uuid;
}

Collection.prototype.parent = function() {
    return this.obj.parent;
}

Collection.prototype.version = function() {
    return this.obj.__version__;
}

Collection.prototype.setName = function(name) {
    this.obj.name = name;
}

Collection.prototype.setName = function(name) {
    this.obj.name = name;
}

Collection.prototype.setParent = function(parent) {
    if( typeof parent === 'object' ) {
	this.obj.parent = parent.uuid;
    }
    else {
	this.obj.parent = parent;
    }
}

Collection.prototype.defineDimension = function(dimName, description) {
    var dimObj = {
	name: dimName,
	description: description
    };
    this.obj.dimensions.push(dimObj);
}

Collection.prototype.registerEntity = function(entity_, label_) {
    var entityObj = {
	name: entity_.__name__,
	uuid: entity_.uuid,
	label: label_
    };

    this.obj.entities.push(entityObj);    
}

Collection.prototype.registerRelationship = function(fromLabel, toLabel, relationshipType)
{
    var relationshipObj = {
	from: fromLabel,
	to: toLabel,
	type: relationshipType
    };
    
    this.obj.relationships.push(relationshipObj);
}

Collection.prototype.accept = function(visitor) {
    return visitor.visit(this);
}

exports.Collection = Collection;
