(function (gnuplot) {

    var code = "";
    var event = new EventLoop(),
	proc = new Process(),
	progname = "C:/Users/thomash/bin/gnuplot/bin/gnuplot.exe";    
    
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
