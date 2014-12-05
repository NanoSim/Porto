/* Connect the entity-instance generator to a datasource */

var entitydb = require('porto.entity').db({
    driver: "mongodb",
    database: "newdb",
    collection: "newcoll"
});

Collection = require('porto.collection').db({
    driver: "mongodb",
    database: "test",
    collection: "collections"
});

Unit       = entitydb.using('unit', '1.0-SNAPSHOT-1');

var c = new Collection();
c.setName('MyColl');
c.setVersion('0.1');

print (c.id, c.name(), c.version());

var u = new Unit('dd13d433-371c-4b06-9136-c663a1a6c729');
var u2 = new Unit();
u2.set({
    abbreviation:         "fy/s",
    code:                 "3780",
    conversionMultiplier: "1.0e0",
    conversionOffset:     "0.0",
    quantityKind:         "quantity:AbsorbedDoseRate",
    symbol:               "Gy/s",
    unit:                 "GrayPerSecond"
});
u2.store();

c.registerEntity(u, 'grayPerSec1');
c.registerEntity(u2, 'grayPerSec2');
c.store();
