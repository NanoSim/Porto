// This example requires that the entity Example_1.0.js has been generated

_ = require('utils.underscore')
include('Example_1.0.js');

var ex1 = new Example(2, 3);
ex1.len = new $Qty('14 m');
 
print(ex1.len + ' in ft = ' + ex1.len.to('ft'));

