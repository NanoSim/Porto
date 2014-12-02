(function (stat){
    var zip = function (xs, ys) {
	var l = Math.min(xs.length, ys.length);
	var ret = [];
	for (var i = 0; i < l; ++i) {
	    ret.push([xs[i], ys[i]]);
	}
	return (ret);
    },    
	sum = function (xs) 
    {
	var acc = 0;
	xs.forEach (function (elem) {
	    acc += elem;
	});
	return acc;
    };
    
    stat.avg = function (xs) {
	if (Object.prototype.toString.call (xs) !== '[object Array]' || xs.length == 0) {
	    throw ("Type error");
	    return undefined;
	}

	if (xs.length == 1) 
	    return xs[0];
	
	var sum = 0;
	xs.forEach (function (x) { sum += x; });
	return sum/xs.length;
    };

    stat.stddev = function(xs) {
	if (Object.prototype.toString.call (xs) !== '[object Array]' || xs.length < 2) {
	    throw ("Type error");
	    return undefined;
	}
	
	return Math.sqrt(function (){
	    var sum = 0;
	    var avg = stat.avg(xs);
	    xs.forEach (function (x) {
		sum += Math.pow(x - avg, 2);
	    });	
	    return sum/(xs.length - 1);
	}());
    };

    stat.leastsqr = function (xs, ys) {
	if (Object.prototype.toString.call (xs) !== '[object Array]' 
	    || Object.prototype.toString.call (ys) !== '[object Array]'
	    || xs.length < 2
	    || ys.length < 2) {
	    throw ("Type error");
	    return undefined;
	}

	var xmean = stat.avg(xs),
	    ymean = stat.avg(ys),
	    xy = zip(xs, ys),	
	    xydiff = xy.slice();
	
	xydiff.forEach (function (elem, index, arr) {
	    arr[index][0] = elem[0] - xmean;
	    arr[index][1] = elem[1] - ymean;
	});
	
	var xdiffsqr = xs.slice();
	xdiffsqr.forEach (function (elem, index, arr) {
	    arr[index] = Math.pow(elem - xmean, 2);
	});
	
	var xydiffProduct = xydiff.slice();
	xydiffProduct.forEach (function (elem, index, arr) {
	    arr[index] = elem[0] * elem[1];
	});
	
	var b1 = sum (xydiffProduct)/sum (xdiffsqr),
	    b0 = ymean - (b1 * xmean);

	return function(x){
	    return b0 + b1*x;
	};
    };

    // R squared
    stat.rsqr = function (xs, ys) {
	if (Object.prototype.toString.call (xs) !== '[object Array]' 
	    || Object.prototype.toString.call (ys) !== '[object Array]'
	    || xs.length < 2
	    || ys.length < 2) {
	    throw ("Type error");
	    return undefined;
	}

	var lsqr = stat.leastsqr(xs, ys),
	    est = xs.slice(),
	    ydiffsqr = ys.slice(),
	    ymean = stat.avg(ys),
	    xmean = stat.avg(xs);
	
	ydiffsqr.forEach (function (elem, index, arr){
	    arr[index] = Math.pow(elem - ymean, 2);
	});

	est.forEach (function (elem, index, arr){
	    arr[index] = lsqr(elem);
	});
	var estMinusMean = est.slice();
	estMinusMean.forEach (function (elem, index, arr){
	    arr[index] = elem - ymean;
	});
	var emmSqr = estMinusMean.slice();
	emmSqr.forEach (function (elem, index, arr){
	    arr[index] = Math.pow(elem, 2);
	});
	var r2 = sum(emmSqr)/sum(ydiffsqr);
	return r2;
    };
    
    // Std error of estimate
    stat.stderror = function (xs, ys) {
	if (Object.prototype.toString.call (xs) !== '[object Array]' 
	    || Object.prototype.toString.call (ys) !== '[object Array]'
	    || xs.length < 2
	    || ys.length < 2) {
	    throw ("Type error");
	    return undefined;
	}
	var lsqr = stat.leastsqr(xs, ys),
	    yest = xs.slice(),
	    ymean = stat.avg(ys),
	    xmean = stat.avg(xs);
	yest.forEach (function (elem, index, arr){
	    arr[index] = lsqr(elem);
	});
	
	var ey = zip(yest, ys);
	ey.forEach (function (elem, index, arr){
	    arr[index] = Math.pow(elem[0] - elem[1], 2);
	});

	var err = Math.sqrt(sum(ey)/(xs.length-2));
	return err;
    };
})(exports);
