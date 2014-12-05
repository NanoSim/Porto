importData = function(extSourceInfo, destInfo)
{
    /* Use a storage factory to get the correct external data driver */
    var reader = createExtStorage(extSourceInfo.driver)
    
    /* Fetch a translator visitor for the two schemas */
    var translator = createTranslator(extSourceInfo.schema, destInfo.schema);

    var externalEntity = reader.fetch(path, ["extsrc", "1.0"]);
    var entity = externalEntity.accept(translator);
    return entity;
}
