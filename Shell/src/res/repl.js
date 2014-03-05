var repl = repl || {}
repl.prompt = "> ";
repl.cont = "+\t";
repl.command = {};

load(":/res/core.js");
fs = require(":/res/filesystem.js");


function showHelp()
{
    var message = "Porto Shell. version 0.0.1\n" +
	"Copyright (c) SINTEF, 2014\n\n" +
	"Shell commands:\n" +
	":h                      This message\n"+
	":ld <file>              Load and evaluate file <file>\n" +
	":chk <file>             Checks the syntax of the program <file>\n" +
	":q                      Quit the shell\n\n";
    print (message);
}


function quit()
{
    repl.exit = true;
    print ("Bye");
}

repl.command['q'] = quit;
repl.command['quit'] = quit;
repl.command['ld'] = load;
repl.command['chk'] = checkSyntax;
repl.command['h'] = showHelp;
repl.command['help'] = showHelp;

function parseCommand(cmd) 
{
    if( cmd != undefined ) {	
	var command = cmd[1];
	if( command !== undefined && repl.command[command] !== undefined) {
	    repl.command[command](cmd[2]);
	    return true;
	} else {
	    print("Unknown command " + command);
	}
    }
    return undefined;
}

function mainrepl()
{
    var s = "";
    while(repl.exit === undefined) {
	try {
	    if( s.length == 0 ) writeline(repl.prompt);
	    s += readline();
	    cmd = s.match(/^[\:]([a-zA-Z]*)\s?(\S*)/);
	    if( cmd != undefined ) {
		s = "";
		parseCommand(cmd);
	    } else {
		if( s.trim().length == 0 )
		    continue;
		if( isValidSyntax(s) ) {		 
		    ret = eval(s);
		    s = "";
		    if( ret != undefined)
			print(ret);
		    else
			print('undefined');
		}
		else if( isIncompleteSyntax(s) )
		{
		    writeline(repl.cont);
		}
		else if( isErrorSyntax(s) ) {

		    throw errorMessage(s);
		    
		}
		else {
		    throw "Huh?";
		}
		
	    }
	} catch (err)
	{
	    print(err);
	    s = "";
	}
    }
}

function __main__(args) {
    var message = "Porto Shell. version 0.0.1\n" +
	"Copyright (c) SINTEF, 2014\n\n" +
	"For help, type :help\n";
    print(message);
    mainrepl();
    return 0;
}
