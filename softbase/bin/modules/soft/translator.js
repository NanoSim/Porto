(function (translator){
    translator.translate = function(from, to) {
	
	var trModuleName = 'soft.translator.' +
		(from.__name__ + "_" + from.__version__ + "_" +
		 to.__name__ + "_" + to.__version__).replace(/\./g, "_");

	if (isValidModule(trModuleName)) {
	    return require(trModuleName).translate(from, to);
	} else {
	    console.error("no valid translator found: " + trModuleName);
	    return undefined;
	}	
    };
})(exports);
