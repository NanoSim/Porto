(function (translator){
    function Translator(arg){
	this.source = arg;
    }

    Translator.prototype.visit = function(dest) {
	dest.name = 'myname';
	dest.data = this.source.data;
    };
    
    translator.translate = function(from, to) {
	var t = new Translator(from);
	to.accept(t);
    };
    return translator;
})(exports);
