Entity           = require('soft.entity').Entity;
createTranslator = require('soft.visitor.translator').createTranslator;

function ExternalStorage(obj)
{
    this.url = obj.url;
    this.driver = obj.driver;
}

ExternalStorage.create = function(obj)
{
    return new ExternalStorage(obj);
}

ExternalStorage.prototype.connect = function(obj)
{
    print( this.url, this.driver );
    return true;
}


/*!
 * createEntityFromExternalSource
 * Creates an entity initialized from an external source
 * @tparam Uuid id Unique identifier of the source data
 * @tparam Storage storage The storage context class for the external resource
 * @tparam StringVector sourceInfo Contains the name and version of the source entity
 * @tparam StringVector destinationInfo Contains the name and version of the destination entity
 */
function createEntityFromExternalSource(id, storage, sourceInfo, destinationInfo)
{
    var logger = storage.fetch(sourceInfo, id);
    var translator = createTranslator(sourceInfo, destinationInfo);
    var entity = new Entity( logger.accept(translator) );
    return entity;
}

exports.ExternalStorage = ExternalStorage;
exports.createEntityFromExternalSource = createEntityFromExternalSource;
