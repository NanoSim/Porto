#!/usr/bin/env softshell

var entity  = require('porto.entity');
Simple = entity.using('simple','ns','0.1');

__main__=function(args)
{
    try {
        var e = new Simple("{2d470115-4c56-4c3a-ab76-e6857daac2c8}");
        e.a = 6;
        e.avec = [1,2,3,4];
        e.mtx = [[[1,2,3],[4,5,6],[7,8,9]],
                 [[1,2,3],[4,5,6],[7,8,9]],
                 [[1,2,3],[4,5,6],[7,8,9]]]

        var storage = new soft.Storage("mongo2", "mongodb://localhost", "db=porto;coll=clean");
        e.save(storage);
    } catch (err) {
        print (err);
    }        
}
