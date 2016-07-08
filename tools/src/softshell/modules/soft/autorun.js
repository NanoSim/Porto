function copyIfUndefined(from, to, element) {
    "use strict";
    if (from === undefined )
	throw ("Undefined object");

    if (element === undefined)
	throw ("Undefined element");

    if( from[element] !== undefined ) {
	if( to[element] === undefined ) {
	    to[element] = from[element];
	}
    }
}

function AutoRun() {
}

AutoRun.run = function(obj, parent) {
    "use strict";

    if( obj === undefined ) 
	throw ("Undefined Object");

    if( obj.name === undefined) 
	throw ("Name not defined from object");

    if( parent === undefined && obj.version === undefined) 
	throw ("Version not defined from object");

    if( typeof(obj.subworkflow) !== 'undefined' ) {
	if( parent !== undefined ) {
	    copyIfUndefined(parent, obj, "workflow");
	    copyIfUndefined(parent, obj, "version");
	}
        for( var i = 0; i < obj.subworkflow.length; ++i ) { 
            exports.runInherit(obj.subworkflow[i], obj);
        }
    }
    if( obj.workflow !== undefined ) {
        for( var i = 0; i < obj.workflow.length; ++i ) {
            obj.workflow[i](obj);
//	    print("waiting");
//	    sys.waitForFinished();
        }

    }
    else if( parent.workflow !== undefined ) {
        for( var i = 0; i < parent.workflow.length; ++i ) {
            parent.workflow[i](obj);
//	    sys.waitForFinished();
        }
    }
    else
	throw(" No workflow defined!");
}

exports = AutoRun;
