(function (capi){
    capi.dims_to_ptr = function(rank) {
	var ret = new String();
	for (var i = 0; i < rank; ++i) {
	    ret += "*";
	}
	return ret;
    };
    
    capi.type_to_c = function(metaType) {
	switch(metaType){
	case 'int8':
	    return "int8_t";
	case 'uint8':
	    return "uint8_t";
	case 'int16':
	    return "int16_t";
	case 'uint16':
	    return "uint16_t";
	case 'int':
	case 'integer':
    	case 'int32':
	    return "int32_t";
	case 'uint32':
	    return "uint32_t";
    	case 'int64':
	    return "int64_t";
	case 'uint64':
	    return "uint64_t";
	case 'float':
	    return "float";
	case 'double':
	    return "double";
	case 'bool':
	case 'boolean':
	    return "bool";
	case 'string':
        case 'string_list':
	    return "softc_string_s";
        case 'blob':
	    return "softc_bytearray_s";
	default:
	    return "undefined";
	};	
    };
    return capi;
})(exports);
