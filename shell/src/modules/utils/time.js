Time = function() {
    this.started = (new Date()).getTime();
};

Time.prototype.start = function()
{
    this.started = (new Date()).getTime();
};

Time.prototype.elapsed = function()
{
    var now = (new Date()).getTime();
    return Math.round(((now/1000) - (this.started/1000)) * 1000);
};

Time.prototype.restart = function()
{
    var oldstarted = this.started;
    this.started = (new Date()).getTime();
    return Math.round(((this.started/1000) - (oldstarted/1000)) * 1000);
};


exports.Time = Time;
