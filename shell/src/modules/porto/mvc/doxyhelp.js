/*!
 @param model        Defines the JSON object that contains properties
 @param linecomment  Defines the string that starts a comment line
*/
function paramDoc(model, linecomment) {
    return model.properties.map(function(o) {
	return(linecomment + "\\param[in] " + o.name + " " + o.description);
    }).join("\n  ");
};

/*!
 @param linecomment  Defines the string that starts a comment line 
*/
function brief(linecomment) {
    return linecomment + " \\brief";
};

exports.paramDoc = paramDoc;
exports.brief    = brief;
