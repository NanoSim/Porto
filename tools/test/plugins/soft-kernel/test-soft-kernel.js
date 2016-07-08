var storage = new soft.Storage("mongo2", "mongodb://localhost", "db=porto;coll=demo");
var entity = new soft.Entity();
var id = entity.id();
var schema = readFile("/home/thomas/Project/soft5/tools/test/plugins/soft-kernel/simple.json");
print (entity.id());

try {
entity.setSchema("{\"name\":\"simple\",\"version\":\"0.1\",\"namespace\":\"ns\",\"dimensions:\":[{\"name\":\"N\"}],\"properties\":[{\"name\":\"a\",\"type\":\"int\"},{\"name\":\"avec\",\"type\":\"int\",\"dims\":[\"N\"]},{\"name\":\"mtx\",\"type\":\"double\",\"dims\":[\"N\",\"N\",\"N\"]}]}");

//entity.setSchema(schema);
entity.setProperty("a", 42);
entity.setProperty("avec", [1,2,3,4]);
entity.setProperty("mtx", [[[1,2,3],[4,5,6],[7,8,9]],
                           [[1,2,3],[4,5,6],[7,8,9]],
                           [[1,2,3],[4,5,6],[7,8,9]]]);


/*
entity.setProperty("b", [1,2,3,4]);
entity.setProperty("c", 3.14);
entity.setProperty("d", [1.1, 2.2, 3.3, 4.4]);
entity.setProperty("e", "Dette er en test");
*/
entity.debug();
    storage.save(entity);
} catch (err) {
    print(err);
}


