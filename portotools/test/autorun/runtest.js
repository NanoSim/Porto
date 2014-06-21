
__main__ = (function()
{
    Time    = require('utils.time');
    AutoRun = require('porto.autorun');
    
    setMessage = function(obj) {
	// Add a message element
	obj.message = 'Hello Porto.AutoRun!';
    };
    
    displayMessage = function(obj) {
	// Print a message if it exists
	if( obj.message !== undefined )
	    print('The message is:', obj.message);
    };

    var workflow = {
	name: 'HelloWorkflow',
	version: '1.0',
	workflow: [setMessage, displayMessage]
    };

    var t = new Time();
    AutoRun.run(workflow);
    print("Finished in: " + t.elapsed() + " milliseconds");
})()
