$ = require('porto.entity');

var example = new $.Entity( $.instance('example', '1.0') );
print (example.name());
print (example.version());
