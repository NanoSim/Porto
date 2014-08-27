#!/usr/bin/env softshell

__main__ = function (args){    
    var name = args[1];
    var viewfile = args[2];
    fs.readFile(name + ".json", function(err, data){
	if (err) throw (err);
	try {
	    generate = require('soft.mvc').create({
		model: JSON.parse(data),
		view: viewfile
	    });
	} catch (e) {
	    console.error(e);
	    return;
	}
	try {
	    console.log(generate({filename: name}));
	} catch (e) {
	    console.error(e);
	}
    });
};
