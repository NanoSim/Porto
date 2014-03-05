/*! instance will try to create an object based on a given meta-entity
 *  (specified by name and version number)
*/

instance = function(name, version) {
    var json = readFile(name + '_' + version + '.json');
    if(json === undefined) throw ("Undefined Enitity");

    var obj = JSON.parse(json);
    var output = {
	__name__: name,
	__version__: version,
	uuid: uuidgen().substr(1,36)
    };
    obj.properties.map(function(prop){
	if(prop.dims !== undefined) {
	    output[prop.name] = [];
	} else if (prop.type === 'object') {
	    output[prop.name] = {};
	} else {
	    output[prop.name] = '';
	}
	output['set_'+prop.name] = function(arg){
	    output[prop.name] = arg; 
	    return arg;
	};
	output['get_'+prop.name] = function(){
	    return output[prop.name];
	};
    });
    return output;
};

exports.instance = instance;
