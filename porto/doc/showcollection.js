#!/usr/bin/env softshell
function uniq(a) {
    var seen = {};
    return a.filter(function(item) {
        return seen.hasOwnProperty(item) ? false : (seen[item] = true);
    });
}

Array.prototype.filterUnique = function(){
    var seen = {};
    return self.filter(function(item) {
        return seen.hasOwnProperty(item) ? false : (seen[item] = true);
    });
}

__main__ = function(args)
{
    if (args < 2) {
        print("usage " + args[0] + " <uuid>");
    }
    var uuid = args[1];

    var storage = new soft.Storage("mongo2", "mongodb://localhost", "db=porto;coll=demo");
    var collection = new porto.Collection(uuid);
    storage.load(collection);

   
    uniq(collection.findRelations("Entity", "^is-a")).forEach(function(label){
        print ("+--- " + label + "\t" +
               collection.entityName(label) + ":" + collection.entityId(label));
    });
}
