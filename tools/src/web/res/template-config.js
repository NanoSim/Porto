with (configure) {
    // Set server port number
    setListenPort(8080);

    // TODO: Uncomment the line below and replace <path-to-root-www-folder>
    // with a root folder path (e.g. /var/www)
//    setDocumentRoot("<path-to-root-www-folder>");

    // Set server name
    setServerName("localhost:8080");

    // Set server admin email address (NOT REQUIRED)
    setServerAdmin("admin@localhost");

    // Set error output file
    setErrorLog("error.log");


    // Set default directory index file
    setDirectoryIndex("index.html");
    
    // TODO: Uncomment the line below and replace <path-to> with
    // the path the runime action handler
    // Example: action("soft-script", "/var/www/cgi-bin/soft-cgi 
// action("<action>", "<path-to-runtime-action-handler>");
    
    // TODO: Define handler to be connected to the defined actions
    // addHandler(<action>, ["<file-extension>, ..."])
    //
    // Example: addHandler("soft-script", ["jscript"])
// addHandler("<action>", ["<type>", ...]);

    // Declare MIME types
    addType("text/html",  ["html", "htm"]);
    addType("application/x-javascript-softshell", ["js"]);
    addType("image/png",  ["png"]);
    addType("image/gif", ["gif", "GIF"]);
    addType("image/jpeg", ["jpeg", "jpg", "jpe", "JPG"]);
    addType("image/x-ico", ["ico"]);
    addType("text/css", ["css"]);

    // Set a response string for server requests
    addRequestHeader("Server", "softweb");    
}
