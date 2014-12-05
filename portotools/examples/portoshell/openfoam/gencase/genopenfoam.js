__main__ = function(args) {
    if (args.length < 2)
	return;

    var entity = require('porto.entity').db({
	driver:     'mongodb',
	database:   'porto',
	collection: 'openfoam'});
    
    ControlDict = entity.using('controlDict', '0.1');
    
    var controlDict = new ControlDict(args[1]);
    var generate = require('porto.mvc').create({
	model: controlDict.get(),
	view : 'controlDict.foamjs'});
    
    fs.writeFile('controlDict', generate(), function(err){
	if (err) throw (err);
    });
}
