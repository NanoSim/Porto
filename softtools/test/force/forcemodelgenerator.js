$MVC = require('soft.mvc');

ForceModelGenerator = function(obj) {
    this.obj = obj;
};

ForceModelGenerator["^doc"] = 
"\nThe ForceModelGenerator is an example generator that uses soft.mvc to " +
"\ncombine an input object with a template view                           " +
"\n                                                                       " +
"\nprovides:                                                              " +
"\ngenerateForceModelSource                                               " +
"\ngenerateForceModelHeader                                               ";

ForceModelGenerator.prototype.generateForceModelSource = function()
{
    var controller = $MVC.create({
	model: this.obj,
	view: 'dragmodel.cppjs'
    });

    return controller.generate();
};

ForceModelGenerator.prototype.generateForceModelHeader = function()
{
    var controller = $MVC.create({
	model: this.obj,
	view: 'dragmodel.hjs'
    });

    return controller.generate();
};
