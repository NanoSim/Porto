
var json = readFile("func.json");
var obj = JSON.parse(json);

eval(obj.sayHello());
eval(obj.sayBye());
