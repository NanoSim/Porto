__main__ = function (args)
{   
    var metaStorage = require ('meta.js').connect();
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

