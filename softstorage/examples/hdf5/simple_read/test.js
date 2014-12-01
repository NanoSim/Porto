
var h5 = new H5();
h5.create("test.h5");
h5.addGroup("foo");
h5.write("text", "some text");
h5.write("list", ["some text","some other text"]);
h5.write("list2", ["some text","some other text", "even more text"]);
h5.write("foo/nested", [[["a","b"],["c","d"],["e","f"]], [["a","b"],["c","d"],["e","f"]]]);
h5.write("foo/doubles", [[1.2, 3.4], [5.5, 4.3]]);
h5.write("foo/sd", 3.14);
h5.close();

h5.open("test.h5");
h5.datasets().forEach (function(ds) {
    h5.infoDataset2 (ds);
});
//h5.info();
h5.close();
