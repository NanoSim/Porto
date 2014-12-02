(function (report){
    var gplot = require ('soft.utils.gnuplot');
    var createGnuPlot = function (fig, view)
    {
	try {
	    return require('soft.mvc').create({
		model: fig,
		view: view
	    })();
	} catch (err) {
	    console.error(err);
	}
	return undefined;
    };
    
    toPdf = function (obj) {
	var gp = createGnuPlot(obj, 'figure.gnpjs');
	print(gp);
	gplot.start(gp);
	(new Process()).execute("epstopdf", [obj.name + ".eps"]);
    };
    
})(exports);
