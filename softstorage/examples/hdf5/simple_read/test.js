
var h5 = new H5();
h5.create("test.h5");
h5.addGroup("foo");
h5.write("text", "some text");
h5.write("test42", [1.0, 2.0, 3.0, 4.0, 5.0]);

h5.write("list", ["some text","some other text", "even some more"]);
h5.write("list2", ["some text","some other text", "even more text"]);
h5.write("list3", [["a", "b"],["c","d"]]);
h5.write("foo/nested", [[["a","b"],["c","d"],["e","f"]], [["a","b"],["c","d"],["e","f"]]]);
h5.write("foo/doubles", [[1.2, 3.4], [5.5, 4.3]]);
h5.write("foo/tripl", [[[1,2,3],[4,5,6]],[[7,8,9],[10,11,12]]]);
h5.write("foo/sd", 3.14);

h5.close();

h5.open("test.h5");
var ds = h5.readArray("test42");
var ss = h5.readStringArray("list");
print (ds, '\n', ss);
h5.infoDataset2("foo/tripl");

h5.close();
