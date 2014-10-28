(function (gnuplot) {

    var code = "";
    var event = new EventLoop(),
	proc = new Process(),
	progname = "gnuplot";    

    proc["error(int)"].connect (function(err){
	switch(err) {
	case 0:
	    console.error("The process failed to start. Either the invoked program is missing, or you may have insufficient permissions to invoke the program.");
	    break;
	case 1:
	    console.error("The process crashed some time after starting successfully.");
	    break;
	case 2:	    
	    console.error("The last waitFor...() function timed out. The state of QProcess is unchanged, and you can try calling waitFor...() again.");
	    break;
	case 3:
	    console.error("An error occurred when attempting to write to the process. For example, the process may not be running, or it may have closed its input channel.");
	    break;
	case 4:
	    console.error("An error occurred when attempting to read from the process. For example, the process may not be running.");
	    break;
	default:
	    console.error("An unknown error occurred. This is the default return value of error().");
	    console.error(err);
	}
	event.quit();
    });
    
    proc["readyReadStandardError()"].connect (function (){
	var stderr = proc.readAllStandardError();
	console.error(stderr);
    });
    
    proc["readyReadStandardOutput()"].connect (function (){
	var stdout = proc.readAllStandardOutput();
	console.log(stdout);
    });
    
    proc["started()"].connect (function (){
	print(progname, " started...");
	proc.write(code);
	proc.writeln("exit");
    });
    
    proc["finished(int)"].connect (function (exitCode) {
	print(progname, "ended with exit code", exitCode);
	event.quit();
    });
    
    gnuplot.start = function(program) {
	code = program;
	try {
	    proc.setProgram(progname);
	    proc.start();
	} catch (err) {
	    console.error(err);
	    throw(err);
	}
	
	return event.exec();	
    };

})(exports);
