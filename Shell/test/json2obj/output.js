var _ = require('utils.underscore');
var $Qty = require('utils.quantities');

function Example (n_, m_) {
    this.dimensions = {
	N: n_,
	M: m_
    };
    this.x = new $Qty("m");
    this.y = new $Qty("m");
    this.xs = _.range(n_).map(function(){
	return _.range(m_).map(function() {
	    return new $Qty("m");
	});
    });
};

Example.prototype = {
    setX: function(arg) {
	this.x = new $Qty(arg);
    },
    setY: function(arg) {
	this.y = new $Qty(arg);
    }
};


var example = new Example(2,3);

example.setX("13 m");
print (example.x.add(new $Qty("1 ft")));

print (example.x);

var convert = $Qty.swiftConverter("m", "ft");
var series = _.flatten(example.xs).map(function(a){a.to("ft");});
print(series);



