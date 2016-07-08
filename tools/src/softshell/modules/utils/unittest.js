Time = require('utils.time');

Array.prototype.equals = function (array) {
    // if the other array is a falsy value, return
    if (!array)
        return false;

    if (this.length != array.length)
        return false;

    for (var i = 0, l=this.length; i < l; i++) {
        // Check if we have nested arrays
        if (this[i] instanceof Array && array[i] instanceof Array) {
            // recurse into the nested arrays
            if (!this[i].equals(array[i]))
                return false;       
        }           
        else if (this[i] != array[i]) { 
            // Warning - two different object instances will never be equal: {x:20} != {x:20}
            return false;   
        }           
    }       
    return true;
};

// Hide method from for-in loops
Object.defineProperty(Array.prototype, "equals", {enumerable: false});

assert_true  = {};
assert_false = {};
assert_eq    = {};
assert_neq   = {};

(function (unittest){
    Test = function() {
	this.tests = {};
    };

    Test.prototype.add = function(label, test) {
	this.tests[label] = test;
    };

    Test.prototype.run = function() {
	var scoretable = {success:0,
			  failure:0};
	for (var key in this.tests) {
	    var result = undefined;
	    var msg = undefined;
	    try {
		Time.start();
		this.tests[key]();
		msg = Time.elapsed() + "ms";
		retval = true;
		scoretable.success++;
	    } catch (err) {
		retval = false;
		msg = err;
		scoretable.failure++;
	    } finally {
		print ("\t[" + (retval ? "OK" : "FAILED") + "] - " + key + " - " + msg);
	    }
	}
	print ("Successful tests: (" + scoretable.success + "/"  + (scoretable.success+scoretable.failure) + ")");	
    };
    
    assert_true = function (condition, message) {
	if (!condition) {
	    throw message || "Assertion failed";
	}
	return true;	
    };

    assert_false = function (condition, message) {
	if (condition) {
	    throw message || "Assertion failed";
	}
	return true;
    };
    
    assert_eq = function (a, b, message) {
	if ((a instanceof Array) && (b instanceof Array)) {
	    if (!a.equals(b)) {
		throw message || "Assertion failed (a:" + a + ", b:" + b + ")";
	    }
	}
	else if (a != b) {
	    throw message || "Assertion failed (a:" + a + ", b:" + b + ")";
	}
	return true;
    };
    
    assert_neq = function h(a, b, message) {
	if ((a instanceof Array) && (b instanceof Array)) {
	    if (a.equals(b)) {
		throw message || "Assertion failed (a:" + a + ", b:" + b + ")";
	    }
	}
	else if (a == b) {
	    throw message || "Assertion failed";
	}
	return true;
    };

    unittest.Test = Test;
    return unittest;
})(exports);
