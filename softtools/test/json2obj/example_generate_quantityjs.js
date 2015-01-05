$mvc = require('soft.mvc');

var json = readFile('example_1.0.json');
var obj = JSON.parse(json);

var source = $mvc.create({
    model: obj,
    view: 'javascript_quantities_generator.jsjs'
});

var generated = source.generate();
var filename = obj.name + '_' + obj.version + '.js';
writeFile(obj.name + '_' + obj.version + '.js', generated);
print('wrote: ' + filename);
