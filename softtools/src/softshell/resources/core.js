var soft = soft || {};  

/*
  The find() method returns a value in the array, if an element in the
  array satisfies the provided testing function. Otherwise undefined
  is returned.
  
  See also the findIndex() method, which returns the index of a found
  element in the array instead of its value.x  
*/
if (!Array.prototype.find) {
  Object.defineProperty(Array.prototype, 'find', {
    enumerable: false,
    configurable: true,
    writable: true,
    value: function(predicate) {
      if (this == null) {
        throw new TypeError('Array.prototype.find called on null or undefined');
      }
      if (typeof predicate !== 'function') {
        throw new TypeError('predicate must be a function');
      }
      var list = Object(this);
      var length = list.length >>> 0;
      var thisArg = arguments[1];
      var value;

      for (var i = 0; i < length; i++) {
        if (i in list) {
          value = list[i];
          if (predicate.call(thisArg, value, i, list)) {
            return value;
          }
        }
      }
      return undefined;
    }
  });
}

/*
  The findIndex() method returns an index in the array, if an element
  in the array satisfies the provided testing function. Otherwise -1
  is returned.x  
*/
if (!Array.prototype.findIndex) {
  Object.defineProperty(Array.prototype, 'findIndex', {
    enumerable: false,
    configurable: true,
    writable: true,
    value: function(predicate) {
      if (this == null) {
        throw new TypeError('Array.prototype.find called on null or undefined');
      }
      if (typeof predicate !== 'function') {
        throw new TypeError('predicate must be a function');
      }
      var list = Object(this);
      var length = list.length >>> 0;
      var thisArg = arguments[1];
      var value;

      for (var i = 0; i < length; i++) {
        if (i in list) {
          value = list[i];
          if (predicate.call(thisArg, value, i, list)) {
            return i;
          }
        }
      }
      return -1;
    }
  });
}
/*
  The fill()method fills all the elements of an array from a start
  index to an end index with a static value.  
*/

if (![].fill) {
  Array.prototype.fill = function(value) {

    // Steps 1-2.
    var O = Object(this);

    // Steps 3-5.
    var len = parseInt(O.length);

    // Steps 6-7.
    var start = arguments[1];
    var relativeStart = parseInt(start) || 0;

    // Step 8.
    var k = relativeStart < 0
            ? Math.max(len + relativeStart, 0)
            : Math.min(relativeStart, len);

    // Steps 9-10.
    var end = arguments[2];
    var relativeEnd = end === undefined
                      ? len 
                      : (parseInt(end) || 0);

    // Step 11.
    var final = relativeEnd < 0
                ? Math.max(len + relativeEnd, 0)
                : Math.min(relativeEnd, len);

    // Step 12.
    for (; k < final; k++) {
        O[k] = value;
    }

    // Step 13.
    return O;
  };
}
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

man = function(module) {
    for (var i in module) {
	print(i, ":", module[i]["^doc"]);
    }
}

