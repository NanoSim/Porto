__main__ = function(args) {
    if (args.length < 2)
	return;

    var entity = require('soft.entity').db({
	driver:     'mongodb',
	database:   'soft',
	collection: 'openfoam'});
    
    ControlDict = entity.using('controlDict', '0.1');
    
    var controlDict = new ControlDict(args[1]);
    var generate = require('soft.mvc').create({
	model: controlDict.get(),
	view : 'controlDict.foamjs'});
    
    fs.writeFile('controlDict', generate(), function(err){
	if (err) throw (err);
    });
}
