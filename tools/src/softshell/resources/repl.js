var repl = repl || {};
repl.prompt = "> ";
repl.cont = "+\t";
repl.command = {};

load(":/resources/core.js");

soft_version = "SOFT v" + version() + " ";
soft_license = "GNU LESSER GENERAL PUBLIC LICENSE (v 2.1, February 1999)";

function showHelp()
{
    var message = soft_version + "(" + soft_license + ")\n\n" +
	"Welcome to Soft Shell\n" +
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
		
		if( isIncompleteSyntax(s) || s[s.length-1] == '\\' )
		{
		    if(s[s.length-1] == '\\') {
			s = s.substring(0,s.length-1);
		    }
		    writeline(repl.cont);
		}
		else {		 
		    ret = eval(s);
		    s = "";
		    if(!isQObject(ret) && isObject(ret)) {
			print(JSON.stringify(ret, null, 2));
		    }
		    else if( ret != undefined)
			print(ret);
		    else
			print('undefined');
		}
	    }
	} catch (err) {
	    if (isIncompleteSyntax(s)) continue;
	    print (err);
	    s = "";
	}
    }
}

function __main__() {
    var message = soft_version + "(" + soft_license + ")\n\n" +
	"For help, type :help\n";
    
    print(message);
    mainrepl();
    return 0;
}
