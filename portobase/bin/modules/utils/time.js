(function (time) {
    var started = (new Date()).getTime();

    time.start = function()
    {
	started = (new Date()).getTime();
    };

    
    time.elapsed = function()
    {
	var now = (new Date()).getTime();
	return Math.round(((now/1000) - (started/1000)) * 1000);
    };

    time.restart = function()
    {
	var oldstarted = started;
	started = (new Date()).getTime();
	return Math.round(((this.started/1000) - (oldstarted/1000)) * 1000);
    };
})(exports);
