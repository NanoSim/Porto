#!/usr/bin/env softshell

/*
 * register-entity.sh
 * A utility to commit meta-data into the metadata-database
 */

__main__ = function (args)
{
    if (args.length <= 1) {
	   console.error("fatal error: no input files");
	   return undefined;
       }
    var metaStorage = require ('soft.utils.metastore').connect(
	{
	    uri: 'mongodb://localhost',
	    database: 'meta',
	    collection: 'entities'
	});
    args.shift();
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

