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

function createTranslator(sourceInfo, destinationInfo)
{
    return new TranslatorDemo;
}

exports.createTranslator = createTranslator;
