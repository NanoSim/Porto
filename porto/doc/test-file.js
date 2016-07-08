#!/usr/bin/env
var entity  = require('porto.entity');
porto.File = entity.using('file', 'http:\/\/info.emmc.eu', '0.1-SNAPSHOT-1');

__main__ = function(args)
{
    try {
        var f = new porto.File();
        f.filename = "test.txt";
        f.suffix = ".txt";
        f.size = 102;
        f.data = "AA02";
        var storage = new soft.Storage("mongo2", "mongodb://localhost", "db=porto;coll=heavy");
        f.save(storage);
        print(f.id());
    } catch (err) {
        print(err);
    }

}
