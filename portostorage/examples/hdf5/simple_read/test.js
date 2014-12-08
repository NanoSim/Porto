
var h5 = new H5();
h5.create("test.h5");
h5.addGroup("foo");
h5.write("a", "some text");
h5.write("b", [1.0, 2.0, 3.0, 4.0, 5.0]);

h5.write("c", ["some text","some other text", "even some more"]);
h5.write("d", ["some text","some other text", "even more text"]);
h5.write("e", [["a", "b"],["c","d"]]);
h5.write("foo/a", [[["a","b"],["c","d"],["e","f"]], [["a","b"],["c","d"],["e","f"]]]);
h5.write("foo/b", [[1.2, 3.4], [5.5, 4.3]]);
h5.write("foo/c", [[[1,2,3],[4,5,6]],[[7,8,9],[10,11,12]]]);
h5.write("foo/d", 3.14);

h5.close();

h5.open("test.h5");
h5.read("a");
h5.read("b");
h5.read("c");
h5.read("d");
h5.read("e");
h5.read("foo/a");
h5.read("foo/b");
h5.read("foo/c");
h5.read("foo/d");

h5.close();
