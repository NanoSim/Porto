/*!
 * Macro
 * Declare the Macro module
 * \Class Macro
 */

function defmacro(f)
{
    var fn = function(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) {
	      return eval(f(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9));
    };  
    fn.isMacro = true; 
    return fn;
}

function expandFile (filename, ext) 
{
    var buffer = readFile(filename);
    if( buffer.length > 0 )
	return(expand(buffer, ext));
    return undefined;
};

function bmatch(text) {
    "use strict";
    var buffer = '',
        ch = '',
        skip = false,
        quote = false,
        lvl = 1,
	start = '{',
	end = '}';
    if( text.charAt(0) !== start ) {
        return undefined;
    }
    
    for( var idx = 1; idx < text.length; ++idx ) {
	ch = text.charAt(idx);
	switch(ch) {
	case '\\':
	    skip = true;
	    break;
	case '\"':
	    if(!skip) {
		quote = !quote;
	    } else {
		skip = false;
	    }
	    break;
	case start:
	    if(!skip && !quote) {
		++lvl;
	    } else {
		skip = false;
	    }
	    break;
	case end:
	    if(!skip && !quote) {
		lvl--;
	    } else {
		skip = false;
	    }
	    break;
	default:
	    skip = false;
	    break;
	}
	
	if (lvl === 0) {
	    return [buffer, text.substr(idx+1)];
	} else {
	    buffer += ch;
	}
    }
    return undefined;
};

function isStruct(text) {
    "use strict";
    var re = /^@([\w\.\$\_]+)\s*\(([^\)]*)\)\s*\{/,
	hits = text.match(re);
    if(hits) {
	var rest = text.substr(hits[0].length-1),
            body = bmatch(rest),
            macro = hits[0] + body[0] + '}';
	
	return [macro, text.substr(macro.length)];
    }
    return undefined;
};

function isBlock(text) {
    "use strict";
    var re = /^@({).*/,
	hits = text.match(re);
    if(hits) {
	var body = bmatch(text.substr(1));
	return [body[0], body[1]];
    }
    return undefined;
};

function isValue(text) {
    "use strict";
    var re = /(^@[\w\.\$\_]+\s*\([^\)]*\)[\;]?)|(^@[\w\.\$\_\[\]]+[\;]?)/,
	hit = text.match(re);
    if(hit !== undefined) {
	return [hit[0], text.substr(hit[0].length)];
    }
    return undefined;
};

function expand(text, ext) {
    "use strict";
    var re = /(@[\w\.\$\_]+)|(@[\{])/,
	buffer = '',
	tmp = '',
	idx = text.search(re);

    function ifString(s) {
	if(typeof s === 'object' ||
	   typeof s === 'function' ||
	   typeof s === 'undefined') {
	    return '';
	}
	return s;
    }

    while( idx >= 0 ) {
	if( idx > 0 ) {
	    buffer += ifString(text.substr(0, idx));
	    text = text.substr(idx);
	} 

	if(undefined !== (tmp = isStruct(text))) {   
	    buffer += ifString(matchStruct(tmp[0], ext));
	    text = tmp[1];
	}

	else if( undefined !== (tmp = isBlock(text))) {
	    buffer += ifString(matchBlock(tmp[0], ext));
	    text = tmp[1];
	}
	else if( undefined !== (tmp = isValue(text))) {
	    buffer += ifString(matchValue(tmp[0], ext));
	    text = tmp[1];
	}                
	else {
	    throw "Undefined macro";
	}

	idx = text.search(re);
    }
    buffer += text;
    return(buffer);
};

function matchStruct(exp, ext) {
    "use strict";
    var re = /^@([\w\.\$\_]+)\s*\(([^\)]*)\)[\s\n]*\{/,
	hits = exp.match(re),
	rest = exp.substr(hits[0].length-1);
    return mfunc(hits[1], hits[2], bmatch(rest)[0].replace(/^([\s]*[\n]+)/, '').replace(/\\/g, '\\\\'), ext);
};

function matchBlock(exp, ext) {
    "use strict";
    return vfunc(exp, ext);
}

function matchValue(exp, ext) {    
    "use strict";

    var re = /^@([\w\.\$\_]+\s*\([^\)]*\)[\;]?)|([\w\.\$\_\[\]]+[\;]?)/,
	hits = exp.match(re);
    if( hits === undefined ) {
	return undefined;
    }

    /* function */
    if (hits[1] !== undefined ) {
	return vfunc(hits[1],ext);
    }

    /* symbol */
    if (hits[2] !== undefined) {
	return vfunc(hits[2],ext);
    }

    throw ("How did you end up here?");
    return undefined;
};

function vfunc(expr,ext) {
    "use strict";
    var b = (ext !== undefined ? "var extra=" + JSON.stringify(ext) + ";" : '') +
	    expr;
    return eval(b);
}

function mfunc(fn, expr, body, ext) {
    var b = "{\nvar xs=[],\n" +
	    "extra=" + JSON.stringify(ext) + ";\n\t" +
	    expand + ";\n\t" + 
	    isStruct + ";\n\t" +
	    isValue + ";\n\t" +
	    isBlock + ";\n\t" +
	    matchStruct + ";\n\t" +
	    matchBlock + ";\n\t" +
	    matchValue + ";\n\t" +
	    vfunc + ";\n\t" +
	    mfunc + ";\n\t" +
	    bmatch + ";\n\t" +	    
	    fn + "(" + expr + "){" +
	    "xs.push(expand('" + 
            body.replace(/\n/g,'\\n').replace(/\"/g,'\\\"').replace(/\'/g,'\\\'') + 
            "',extra));};xs.join('');}";
    return eval(b);
}

exports.defmacro = defmacro;
exports.expandFile = expandFile;
exports.expand = expand;
