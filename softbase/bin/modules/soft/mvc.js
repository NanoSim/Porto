(function (mvc){
    var soft = soft || {};  
    mvc.create = function (obj) {
	var expand = require('soft.macro').expand;
	
	if( obj.model === undefined )
	    throw ("Missing model in object");
	
	if( obj.view === undefined )
	    throw ("Missing view in object");
	
	return function(bag){
		var view = readFile(obj.view);
		if (view == undefined)
		    throw ("Illegal file", obj.view);
		
		if( typeof obj.model == 'object' ) {
		    soft.model = obj.model;
		}
		else {
		    var json = readFile(obj.model);
		    if (json == undefined)
			throw ("Illegal file", obj.model);
		    
		    soft.model = JSON.parse(json);
		}
		soft.bag = bag;
		return expand(view);
	};	
    }
})(exports);
