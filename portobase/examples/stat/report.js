(function (report){
    var gplot = require ('porto.utils.gnuplot');
    var createGnuPlot = function (fig, view)
    {
	try {
	    return require('porto.mvc').create({
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
