/*!
 * Translator (visitor pattern)
 */
function TranslatorDemo()
{
    var that = this;
}

TranslatorDemo.prototype.visit = function(entity) 
{    
}

TranslatorDemo.createTranslator = function(sourceInfo, destinationInfo)
{
    return new TranslatorDemo;
}

exports = TranslatorDemo

