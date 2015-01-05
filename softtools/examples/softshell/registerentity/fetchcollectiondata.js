var driverInfo = {
    driver: 'mongodb',
    database: 'soft',
    collection: 'units'
};

/* Entity and Collection creators */
Collection = require('soft.collection').db(driverInfo);

var collectionID = '79fe6b02-7b9e-4339-b238-983333b37552';
var unitLibrary  = new Collection(collectionID);

print (unitLibrary.id);
print (unitLibrary.count());
//print (JSON.stringify(unitLibrary.get()));
print (JSON.stringify(unitLibrary.findInstance('entity1370')));

// Generate something based on this
var generate = require('soft.mvc').create({
    model: unitLibrary.get(),
    view: 'webtemplate.jshtml'
});

var entity = require('soft.entity').db(driverInfo);
Unit = entity.using('unit', '1.0-SNAPSHOT-1');

fs.writeFile('output.html', generate(driverInfo), function(err){
    if (err) throw(err);
});

