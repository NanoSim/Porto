#!/usr/bin/env softshell

var entity = require('porto.entity');
XY = entity.using('xy', 'nanosim', '0.1-SNAPSHOT-1');

__main__=function(args)
{
    try {
        var simpleplot = new XY();
        simpleplot.label = ["X", "Y"];
        simpleplot.data = [[1.0, 1.2],[2.0, 1.8],[3.0, 1.6]];
        var storage = new soft.Storage("mongo2", "mongodb://localhost", "db=porto;coll=xy");
        simpleplot.save(storage);
    } catch (err) {        
        print (err);
    }        
}
