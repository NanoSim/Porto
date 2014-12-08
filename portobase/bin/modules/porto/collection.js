(function (collection){
    collection.db = function(driverInfo) {

	var colldb = require('porto.entity').db(driverInfo);
	
	Collection = colldb.using('Collection', '1.0-RC-3');
	
	
	Collection.prototype.setName = function(name) {
	    this.__name__ = name;
	};
	
	Collection.prototype.name = function() {
	    return this.__name__;
	};
	
	Collection.prototype.setVersion = function(version) {
	    this.__version__ = version;
	};
	
	Collection.prototype.version = function() {
	    return this.__version__;
	};

	Collection.prototype.count = function() {
	    return this.entities.length;
	};

	Collection.prototype.instances = function() {
	    return this.entities.map (function (obj){
		return obj.label;
	    });
	};
	
	Collection.prototype.findInstance = function(label) {
	    return this.entities.filter (function(obj){
		return (obj.label == label);
	    }).shift();
	};

	Collection.prototype.registerRelation = function(_fromLabel, _toLabel, _relation) {
	    this.relations.push({fromLabel: _fromLabel,
				 toLabel  : _toLabel,
				 relation : _relation});
	};
	
	Collection.prototype.registerEntity = function(_entity, _label) {
	    this.entities.push({name   : _entity.__name__, 
				version: _entity.__version__,
				uid    : _entity.id,
				label  : _label});
	}

	Collection.prototype.accept = function(visitor) {
	    return visitor.visit(this);
	}
	
	return Collection.prototype.constructor;
    }
})(exports);
