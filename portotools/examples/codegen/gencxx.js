#!/usr/bin/env portoshell

__main__ = function (args){    
    var name = args[1];
    fs.readFile(name + ".json", function(err, data){
	if (err) throw (err);
	try {
	    generate = require('porto.mvc').create({
		model: JSON.parse(data),
		view: 'entity.cxxjs'
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