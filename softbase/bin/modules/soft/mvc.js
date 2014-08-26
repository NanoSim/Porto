(function (mvc){
    var porto = porto || {};  
    mvc.create = function (obj) {
	var expand = require('porto.macro').expand;
	
	if( obj.model === undefined )
	    throw ("Missing model in object");
	
	if( obj.view === undefined )
	    throw ("Missing view in object");
	
	return function(bag){
		var view = readFile(obj.view);
		if (view == undefined)
		    throw ("Illegal file", obj.view);
		
		if( typeof obj.model == 'object' ) {
		    porto.model = obj.model;
		}
		else {
		    var json = readFile(obj.model);
		    if (json == undefined)
			throw ("Illegal file", obj.model);
		    
		    porto.model = JSON.parse(json);
		}
		porto.bag = bag;
		return expand(view);
	};	
    }
})(exports);
