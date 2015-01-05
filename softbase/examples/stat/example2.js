/* CREATE DATA */

var h5 = new H5();
h5.create("data.h5");
h5.write("c", [[1,2],
	       [2,4],
	       [3,5],
	       [4,4],
	       [5,5]]);
h5.close();

/* READ DATA */
h5.open("data.h5");
var c = h5.read("c");
h5.close();


/* CREATE PLOT */

var xs = c.slice(),
    ys = c.slice();
xs.forEach (function (elem, index, arr){
    arr[index] = elem[0];
});
ys.forEach (function (elem, index, arr){
    arr[index] = elem[1];
});

print (xs);
print (ys);

var lsqry = xs.slice(),
    leastSquare = require('utils.averagej').leastsqr(xs,ys);

lsqry.forEach (function (elem, index, arr){
    arr[index] = leastSquare(elem);
});

print (lsqry);
var report = require('report.js');

