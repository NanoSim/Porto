#!/usr/bin/env portshell

if (!String.prototype.isEmpty) {
    String.prototype.isEmpty = function(){
	return this == "";
    };
}

if (!String.prototype.contains) {
  String.prototype.contains = function() {
    return String.prototype.indexOf.apply(this, arguments) !== -1;
  };
}

parseCsv = function (file)
{
    try {
	var list = [];
	fs.readFile(file, function (err, buff) {
	    var lines = buff.split('\n');
	    lines.forEach (function (l){
		var rows = l.split(";");
		list.push (rows);
	    });	 
	});

	var o = {
	    name: list[0][1],
	    description: list[0][2],
	    version: list[0][3].trim().isEmpty() ? "0.1-SNAPSHOT-1": list[0][3].trim(),
	    dimensions: [],
	    properties: []
	};

	var parseProps = function(xs) {
	    if (xs.length < 6) return undefined;
	    var property = {
		name: xs[1],
		type: xs[2],
		unit: xs[3],
		dims: (function(){
		    if(xs[4].trim().isEmpty()) return [];
		    var d = xs[4].split(",");
		    d.forEach (function (element, index, array) {
			array[index] = element.trim();
		    });
		    return d;
		})(),
		description: xs[5]
	    };
	    return property;
	};

	var parseDims = function(xs) {
	    if (xs.length < 2) return undefined;
	    var dims = {
		name: xs[1],
		description: xs[2]
	    };
	    return dims;
	};
	
	var PROPS = 1,
	    DIMS = 2,
	    lastHdr = 0;

	for (var i = 1; i < list.length; ++i) {
	    list[i].forEach (function (element, index, array){
		array[index] = element.trim();
	    });
	    var hdr = list[i][0].toLowerCase();
	    if (hdr.contains("properties")) {
		lastHdr = PROPS;
	    } else if (hdr.contains("dimensions")) {
		lastHdr = DIMS;
	    } 

	    if( lastHdr == PROPS) {
		var p = parseProps (list[i]);
		if (p != undefined) {
		    o.properties.push (p);
		}
	    } else if (lastHdr == DIMS) {
		var d = parseDims (list[i]);
		if (d != undefined) {
		    o.dimensions.push (d);
		}
	    }	    
	}

	return o;
    } catch (err) {
	print (err);
    }
    return {};
};

__main__ = function (args)
{
    var o = parseCsv (args[1]);
    var outputFile = fs.baseName(args[1]) + ".json";
    fs.writeFile(outputFile, JSON.stringify(o, undefined, 2));
};
