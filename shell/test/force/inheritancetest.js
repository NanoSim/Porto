

// Defining test one, prototypal
var testOne = function () {};
testOne.prototype = (function () {
    var me = {}, privateVariable = 42;
    me.someMethod = function () {
        return privateVariable;
    };

    me.publicVariable = "foo bar";
    me.anotherMethod = function () {
        return this.publicVariable;
    };

    return me;

})();


// Defining test two, function

var testTwo = function()
{
    var me = {}, 
	privateVariable = 42;
    me.someMethod = function () {
        return privateVariable;
    };

    me.publicVariable = "foo bar";
    me.anotherMethod = function () {
        return this.publicVariable;
    };

    return me;
};



// Proving that both techniques are functionally identical
var resultTestOne = new testOne();
var resultTestTwo = new testTwo();

print(
    resultTestOne.someMethod(), // Should print 42
    resultTestOne.publicVariable // Should print "foo bar"
);

print(
    resultTestTwo.someMethod(), // Should print 42
    resultTestTwo.publicVariable // Should print "foo bar"
);



// Performance benchmark start
var stop, start, loopCount = 1000000;

// Running testOne
start = (new Date()).getTime(); 
for (var i = loopCount; i>0; i--) {
    new testOne();
}
stop = (new Date()).getTime();

print('Test one took: '+ Math.round(((stop/1000) - (start/1000))*1000) +' milliseconds');



// Running testTwo
start = (new Date()).getTime(); 
for (var i = loopCount; i>0; i--) {
    new testTwo();
}
stop = (new Date()).getTime();

print('Test two took: '+ Math.round(((stop/1000) - (start/1000))*1000) +' milliseconds');
