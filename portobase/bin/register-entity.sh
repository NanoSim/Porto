#!/usr/bin/env portoshell

/*
 * register-entity.sh
 * A utility to commit meta-data into the metadata-database
 */

__main__ = function (args)
{
    if (args.length == 0) {
	   console.error("fatal error: no input files");
	   return undefined;
       }
    var metaStorage = require ('porto.utils.metastore').connect(
	{
	    uri: 'mongodb://localhost',
	    database: 'meta',
	    collection: 'entities'
	});
    args.forEach(function(file){
	fs.readFile(file, function(err, data) {
	    if (err) {
		print ("error:", err);
		return;
	    }
	    if (!metaStorage.store (data)) {
		print ("Failed to write data");
	    }
	});
    });
}

