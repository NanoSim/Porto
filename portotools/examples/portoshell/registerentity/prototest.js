(function (proto){
    
    function Proto(id)
    {
	this.id = id;
    };

    Proto.prototype.sayHello = function() {
	print("hello");
    };
   
    proto.constructor = function() {
	return Proto.prototype.constructor;
    };
})(exports);
