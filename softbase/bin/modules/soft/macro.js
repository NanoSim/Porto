(function (macro){
    /** \fn bmatch(text)
     * A bracket-matcher
     *
     * The bmatch function (bracket match) will check that the first
     * character of the text buffer is the start-character '{' bmatch will
     * then search through the buffer until it reaches the end-character
     * '}'. bmatch allows nested brackets at multiple levels, and will
     * ignore quoted brackets and brackets that resides inside of strings (\" \")
     */

    Macro = function(){};
    
    /** \fn expandFile(filename,ext)
     *  \param string filename Input file
     *  \param object ext An optional argument that allows a custom object to be submitted to the expand function
     */
    macro.expandFile = function (filename, ext) 
    {
	var buffer = readFile(filename);
	if( buffer.length > 0 )
	    return(macro.expand(buffer, ext));
	return undefined;
    };
    Macro.expandFile = macro.expandFile;

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

    /** \fn isStruct(text)
     *  the isStruct function will match if the text is in this format:
     *  @<keyword>(<parameters>){<body>}
     *
     *  Example:
     *  @for (var i = 0; i < 10; ++i) { I can count to $i }
     *
     *  \param string text Input buffer
     */

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

    /** \fn isBlock(text)
     * the isBlock will match if the text is in this format:
     * @{ <body> }
     *
     * \param string text Input buffer
     */
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

    /** \fn isValue(text)
     * the isValue will match if the text is in this format:
     * @<keyword>
     * @<keyword>;
     * @<keyword-with-dot-or-underscore>
     * @<keyword-with-dot-or-underscore>
     * @<some-function>(<arg>);
     *
     * \param string text Input buffer
     */
    function isValue(text) {
	"use strict";
	var re = /(^@[\w\.\$\_]+\s*\([^\)]*\)[\;]?)|(^@[\w\.\$\_\[\]]+[\;]?)/,
	    hit = text.match(re);
	if(hit !== undefined) {
	    return [hit[0], text.substr(hit[0].length)];
	}
	return undefined;
    };

        /** \fn expand(text, ext)
     *
     * expand will take the input string 'text' and an optional object ext
     * and evaluate expressions that matches the isStruct, isBlock or
     * isValue patterns. The result from the evalution with replace the
     * input-expression in the text. Text that doesn't match the patterns is
     * left as-is.
     * 
     * It is also possible to pass an object to expand. This will make the
     * object visible from inside the expander through the named object 'extra'
     * example:
     * expand("My name is @extra.name", {name: "John Doe"});
     */
    macro.expand = function(text, ext) {
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

    Macro.expand = macro.expand;


    macro.defmacro = function(f)
    {
	var fn = function(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) {
	    return eval(f(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9));
	};  
	fn.isMacro = true; 
	return fn;
    }

    /** \fn matchStruct(exp, ext)
      Matches a struct pattern and sends the arguments into mfunc
    */
    function matchStruct(exp, ext) {
	"use strict";
	var re = /^@([\w\.\$\_]+)\s*\(([^\)]*)\)[\s\n]*\{/,
	    hits = exp.match(re),
	    rest = exp.substr(hits[0].length-1);
	return mfunc(hits[1], hits[2], bmatch(rest)[0].replace(/^([\s]*[\n]+)/, '').replace(/\\/g, '\\\\'), ext);
    };

    /** \fn matchBlock(exp, ext)
     * Sends the entire expression to vfunc for evaluation.
     */
    function matchBlock(exp, ext) {
	"use strict";
	return vfunc(exp, ext);
    }

    /** \fn matchValue(exp, ext)
     * 
     * Check if the expresiion is a function or a symbol, and pass the
     * hits on to vfunc.
     */
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
    };

    /** \fn vfunc(expr, ext)
     * 
     * The vfunc is a simple expander, as it only has to evaluate the
     * expression given. if 'ext' is given, a local variable named "extra"
     * is created containing the ext-object stringified.
     */
    function vfunc(expr,ext) {
	"use strict";
	var b = (ext !== undefined ? "var extra=" + JSON.stringify(ext) + ";" : '') +
	    expr;
	return eval(b);
    }

    /** \fn mfunc(fn, expr, body, ext)
     *
     * The mfunc (macro function) is the heart of the macro expander
     * utility for expanding nested structs. It creates a buffer that
     * contains all the function definitions (defined in this scope),
     * along with a function that is defined by the struct
     * @<function>()... When the buffer is evaluated, the structures
     * defined in the body itself gets evaluated, sharing the scope of the
     * level before, this allowing for nested structures like this:
     * 
     * @for(var i = 0; i < 10; ++i) {
     *   @for(var j = 0; i < 10; ++j) {
     *     This is tabel[@i][@j]
     *   }
     * }
     */
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
	    "xs.push(Macro.expand('" + 
            body.replace(/\n/g,'\\n').replace(/\"/g,'\\\"').replace(/\'/g,'\\\'') + 
            "',extra));};xs.join('');}";
	return eval(b);
    }
    
    return macro;
})(exports);
