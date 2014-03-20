expand = require(':/res/macro.js').expand;

function typeName(model) {
    return( "T" + model.name[0].toUpperCase() + model.name.substr(1) );
};

function classDescription(model) {
    return( (model.description !== undefined ? model.description : '' ) );
};

function className(model) {
    return model.name;
};

function classVersion(model) {
    return model.version;
}

function ClassName(model) {
    return className(model).replace(/(?:^|\s)\S/, function(a){
	return a.toUpperCase(); 
    });
};

function typeTable(table) {
    return function(t) {
	return table[t];
    };
};

var fortranType = typeTable({
    "double":  "real(dp)",
    "integer": "integer",
    "float": "real(sp)"
});

function propertyDeclarationIn(model) { 
    return model.properties.map(function(o) {
	var typedecl = expand( "@{fortranType(extra.type)}, intent(in) :: @extra.name", o);
	var dimdecl = (o.dim !== undefined ? '(' + o.dim.split(',').map(function(){return(':');}).join(',') + ')' : '');
	return typedecl + dimdecl;
    }).join("\n    ");
};

function propertyDeclaration(model) { 
    return model.properties.map(function(o) {
	var typedecl = expand( "@{fortranType(extra.type)} @if(extra.dim){, allocatable} :: @extra.name", o);
	var dimdecl = (o.dim !== undefined ? '(' + o.dim.split(',').map(function(){return(':');}).join(',') + ')' : '');
	var docdecl = (o.description !== undefined ? ' ! ' + o.description : '');
	return typedecl + dimdecl + docdecl;
	
    }).join("\n    ");
};

function propertyInitialization(model) {
    return model.properties.map(function(o) {
	return expand( "this % @extra.name = @extra.name", o );
       }).join("\n    ");
};


function propertyList(model) {
    return model.properties.map(function(o) {
	return expand( "@extra.name", o );
    }).join(" &\n    &, ");
};

function dimensionDeclaration(model) {
    return model.dimensions.map(function(o) {
	return expand("integer :: @extra.name    ! @extra.description", o);	
    }).join("\n    ");
};

function dimensionDeclarationIn(model) {
    return model.dimensions.map(function(o) {
	return expand("integer, intent(in) :: @extra.name    ! @extra.description", o);	
    }).join("\n    ");
};

function dimensionList(model) {
    return model.dimensions.map(function(o) {
	return o.name;
    }).join(', ');
};

function dimensionAllocate(model) {
    return model.properties.map(function(o) {
	if(o.dim !== undefined) {
	    return expand("allocate(this%@{extra.name}(@{extra.dim}))", o);
	};
	return undefined;
    }).filter(function(str){return(str !== undefined);}).join("\n    ");
};

exports.typeName = typeName;
exports.className = className;
exports.ClassName = ClassName;
exports.classDescription = classDescription;
exports.classVersion = classVersion;
exports.fortranType = fortranType;
exports.propertyDeclaration = propertyDeclaration;
exports.propertyDeclarationIn = propertyDeclarationIn;
exports.propertyInitialization = propertyInitialization;
exports.propertyList = propertyList;
exports.dimensionDeclaration = dimensionDeclaration;
exports.dimensionDeclarationIn = dimensionDeclarationIn;
exports.dimensionList = dimensionList;
exports.dimensionAllocate = dimensionAllocate;
