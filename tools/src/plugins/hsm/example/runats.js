#!/usr/bin/env softshell

__main__ = function (args)
{
    var eventLoop = new EventLoop();   
    try {
	var wf = JSON.parse(readFile(args[1]));
	var sc = JSON.parse(readFile(args[2]));

	Ats.Workflow.run (wf, sc, function(watcher) {
	    watcher["started()"].connect (function(){
		console.log ("Process started");
	    });

	    watcher["finished()"].connect (function(){
		console.log ("Done!");
		eventLoop.quit();
	    });

	    watcher["resultReadyAt(int)"].connect (function (index){
		console.log ("Workflow #" + index + " finished");
	    });
	});
    } catch (err) {
	print (err);
    }
    return eventLoop.exec();
}
