var porto = porto || {};  
var $macro = require(":/res/macro.js");
var expand = $macro.expand;
var expandFile = $macro.expandFile;
var defmacro = $macro.defmacro;
var macrodebug = $macro.debug;

if( !String.prototype.trim) {
    String.prototype.trim = function(){return this.replace(/^\s+|\s+$/g, '');};
}

if( !String.prototype.regexIndexOf) {
    String.prototype.regexIndexOf = function(regex, startpos) {
	var indexOf = this.substring(startpos || 0).search(regex);
	return (indexOf >= 0) ? (indexOf + (startpos || 0)) : indexOf;
    };
}

function doc(fn) {
    if( fn["^doc"] !== undefined )
	print( fn["^doc"] );
    return "\n";
}


function zip() { 
    var args = [].slice.call(arguments);
    var shortest = args.length == 0 ? [] : args.reduce(function(a,b) {
        return a.length < b.length ? a : b;
    });
    
    return shortest.map( function(_,i) {
        return args.map(function(array) {
            return array[i];
        });
    });
}
   


function macroexpand(m)
{
    var args = [];
    for( var i = 1; i < arguments.length; ++i ) {
	args.push(arguments[i]);
    }
    print(m.body);
    print(m.args);
    print(args);
}

function map(f, l) {
    var results = [];
    return (function() {
        for(var i in l) {
            results.push(f(l[i]));
        }
        return results;
    })();
}

// Java style iterator
//function Iterator(list) {
Array.prototype.Iterator = function() {
    var o = this;
    var index = -1;
    var count = this.length;

    this.toFront = function() {
	index = -1;
    };

    this.toBack = function() {
	index = count;
    };

    this.peekNext = function() {
	return o[index + 1];
    };

    this.peekPrevous = function() {
	return o[index - 1];
    };

    this.hasNext = function() {
	return (index < count - 1);
    };

    this.hasPrevious = function() {
	return (index > 0);
    };

    this.previous = function() {
	return o[--index];
    };

    this.next = function() {
	return o[++index];
    };
    
    this.count = function() {
	return count;
    };
};
