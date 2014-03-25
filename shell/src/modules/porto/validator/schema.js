module = {};

(function(){
    function Module() {
	if (!(this instanceof Module))
	    return new Module();    
    
	this.env = require('utils.jjv');
	
	this.env.addSchema('entity', {"type":"object","$schema": "http://json-schema.org/draft-03/schema","id": "http://jsonschema.net","required":false,"properties":{ "description": { "type":"string", "id": "http://jsonschema.net/description", "required":false }, "dimensions": { "type":"array", "id": "http://jsonschema.net/dimensions", "required":false, "items": { "type":"object", "id": "http://jsonschema.net/dimensions/0", "required":false, "properties":{ "description": { "type":"string", "id": "http://jsonschema.net/dimensions/0/description", "required":false }, "name": { "type":"string", "id": "http://jsonschema.net/dimensions/0/name", "required":true } } } }, "domain": { "type":"string", "id": "http://jsonschema.net/domain", "required":false }, "enums": { "type":"array", "id": "http://jsonschema.net/enums", "required":false, "items": { "type":"object", "id": "http://jsonschema.net/enums/0", "required":false, "properties":{ "description": { "type":"string", "id": "http://jsonschema.net/enums/0/description", "required":false }, "name": { "type":"string", "id": "http://jsonschema.net/enums/0/name", "required":true }, "values": { "type":"array", "id": "http://jsonschema.net/enums/0/values", "required":true, "items": { "type":"string", "id": "http://jsonschema.net/enums/0/values/0", "required":false } } } } }, "name": { "type":"string", "id": "http://jsonschema.net/name", "required":true }, "properties": { "type":"array", "id": "http://jsonschema.net/properties", "required":true, "items": { "type":"object", "id": "http://jsonschema.net/properties/0", "required":false, "properties":{ "description": { "type":"string", "id": "http://jsonschema.net/properties/0/description", "required":false }, "dims": { "type":"string", "id": "http://jsonschema.net/properties/0/dims", "required":false }, "name": { "type":"string", "id": "http://jsonschema.net/properties/0/name", "required":true }, "type": { "type":"string", "id": "http://jsonschema.net/properties/0/type", "required":true }, "unit": { "type":"string", "id": "http://jsonschema.net/properties/0/unit", "required":false } } } }, "version": { "type":"string", "id": "http://jsonschema.net/version", "required":true } }, required: ['name', 'version', 'properties'] } );
	return undefined;
    };
    
    
    Module.prototype = {
	validate: function (obj) {
	    var o = undefined;
	    if( obj === undefined )
		throw ("Missing object argument");
	    
	    if( typeof obj === 'object') {
		o = obj;
	    } else {
		var json = readFile(obj);
		if( json === undefined )
		    throw ("Illegal filename", obj);

		o = JSON.parse(json);
	    }
	    var errors = this.env.validate('entity', o);
	    if( !errors ) {
		print('Entity has been validated.');
		return true;
	    } else {
		print('Failed to validate with error object ' + JSON.stringify(errors));
		return false;
	    }
	}
    };    

    module.exports = Module;

}).call(this);

exports = module.exports();
