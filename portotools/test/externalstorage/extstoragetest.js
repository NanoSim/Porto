/*!
 * Create an entity instantiated from an external source
 */
createEntityFromExternal = function(info) {
    var storage = ExternalStorage.addExternalStorageDriver(info.driver);
    var driver  = storage.driver();
    if (!driver.open(info.url))
	throw("Cannot open ", info.url);
    var entity  = storage.fetch(info.name, info.version);
    return entity;
}

__main__ = function(args)
{
    /* create a entity representing the external data */
    var logdata = createEntityFromExternal({
	name   : 'logdata',
	version: '0.0.1',
	driver : 'logdat',
	url    : 'myfile.log'
    });

    /* fetch a translator that convert the contents of logdata:0.0.1 
       to an internal particle:1.0 entity */
    var translator = TranslatorFactory.create({
	source_name   : 'logdata',
	source_version: '0.0.1',
	dest_name     : 'particle',
	dest_version  : '1.0'
    });

    /* create our particle:1.0 instance */
    var particle = logdata.accept(translator);
}
