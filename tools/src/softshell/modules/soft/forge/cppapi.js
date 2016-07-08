(function (cppapi){
    cppapi.type_to_cpp = function(metaType, rank) {
	rank = typeof rank !== 'undefined' ? rank : 0;
	switch(metaType){
	case 'int8':
	    return "StdInt8";
	case 'uint8':
	    return "StdUInt8";
	case 'int16':
	    return "StdInt16";
	case 'uint16':
	    return "StdUInt16";
	case 'int':
	case 'integer':
    	case 'int32':
	    switch(rank) {
	    case 0:
		return "StdInt";
	    case 1:
		return "StdIntArray";
	    case 2:
		return "StdIntArray2D";
	    case 3:
		return "StdIntArray3D";
	    case 4:
		return "StdIntArray4D";
	    default:
		throw("Unimplemented rank for int (" + rank + ")");
	    }
	case 'uint32':
	    return "StdUInt";
    	case 'int64':
	    return "StdInt64";
	case 'uint64':
	    return "StdUInt64";
	case 'float':
	    return "StdFloat";
	case 'double':
	    switch(rank) {
	    case 0:
		return "StdDouble";
	    case 1:
		return "StdDoubleArray";
	    case 2:
		return "StdDoubleArray2D";
	    case 3:
		return "StdDoubleArray3D";
	    case 4:
		return "StdDoubleArray4D";
	    default:
		throw("Unimplemented rank for double");
	    };
	case 'bool':
	case 'boolean':
	    return "StdBool";
	case 'blob':
	    return "StdBlob";
	case 'string':
	    switch(rank) {
	    case 0:
		return "StdString";	
	    case 1:
		return "StdStringList";
	    default:
		throw ("Unimplemented rank for string");
	    };	
	    break;
	default:
	    throw ("Illegal type: " + metaType);
	};
	return undefined;
    };
    return cppapi;
})(exports);
