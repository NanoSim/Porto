(function (fapi){
    fapi.dims_to_ptr = function(rank) {
	var xs = [];
	for (var i = 0; i < rank; ++i) {
	    xs.push(':');
	}
	return xs.join(',');
    };
    
    fapi.type_to_f = function(metaType) {
	switch(metaType){
	case 'int8':
	case 'uint8':
	    return "integer(c_int8_t)";
	case 'int16':
	case 'uint16':
	    return "integer(c_int16_t)";
	case 'int':
	case 'integer':
    	case 'int32':
	case 'uint32':
	    return "integer(c_int32_t)";
    	case 'int64':
	case 'uint64':
	    return "integer(c_int64_t)";
	case 'float':
	    return "real(c_float)";
	case 'double':
	    return "real(c_double)";
	case 'bool':
	case 'boolean':
	    return "logical(c_bool)";
	default:
	    return "undefined";
	};	
    };
    return fapi;
})(exports);
