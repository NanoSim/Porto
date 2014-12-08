var stat = require ('utils.averagej');
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

var toPdf = function (obj) {
    var gp = createGnuPlot(obj, 'figure.gnpjs');
    print(gp);
    gplot.start(gp);
    (new Process()).execute("epstopdf", [obj.name + ".eps"]);
    
};

var xs = [1,2,3,4,5];
var ys = [2,4,5,4,5];

var fn = stat.leastsqr(xs,ys);
var ydf = xs.slice();
ydf.forEach (function (elem, index, arr){
    arr[index] = fn(elem);
});

var o = {
    title: "Some title",
    name: "Testing",
    xsize: 1.0,
    ysize: 1.0,
    xlabel: 'X',
    ylabel: 'Y',
    curves: [
	{
	    title: 'line 1',
	    type: 'dotted line',
	    color: 'blue',
	    width: 2,
	    xdata: xs,
	    ydata: ys
	},
	{
	    title: 'fitted',
	    type: 'line',
	    color: 'green',
	    width: 2,
	    xdata: xs,
	    ydata: ydf
	}
    ]
};

toPdf (o);
