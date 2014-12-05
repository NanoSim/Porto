$create = require('porto.mvc').create;

ForceModelGenerator = function(obj) {
    this.obj = obj;
};

ForceModelGenerator.prototype.generateForceModelSource = function()
{
    var controller = $create({
	model: this.obj,
	view: 'dragmodel.cppjs'
    });

    return controller.generate();
};

ForceModelGenerator.prototype.generateForceModelHeader = function()
{
    var controller = $create({
	model: this.obj,
	view: 'dragmodel.hjs'
    });

    return controller.generate();
};
