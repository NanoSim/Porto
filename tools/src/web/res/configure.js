with (configure) {
    setListenPort(8080);
    setDocumentRoot("/var/www");
    setServerName("localhost:8080");
    setServerAdmin("admin@localhost");
    setErrorLog("error.log");

    action("soft-script", "/var/www/cgi-bin/soft-cgi");
    addHandler("soft-script", ["jscript"]);    
    addType("text/html",  ["html", "htm"]);
    addType("text/plain", ["txt"]);
    addType("text/richtext", ["rtx"]);
    addType("text/tab-separated-values", ["tsv"]);
    addType("application/pdf", ["pdf"]);
    addType("application/rtf", ["rtf"]);
    addType("application/zip", ["zip"]);
    addType("application/x-javascript-softshell", ["js"]);
    addType("image/png",  ["png"]);
    addType("image/gif", ["gif", "GIF"]);
    addType("image/ief", ["ief"]);
    addType("image/jpeg", ["jpeg", "jpg", "jpe", "JPG"]);
    addType("image/tiff", ["tiff","tif"]);
    addType("image/x-ico", ["ico"]);
    addType("image/x-cmu-raster",  ["ras"]);
    addType("image/x-portable-anymap",  ["pnm"]);
    addType("image/x-portable-bitmap",  ["pbm"]);
    addType("image/x-portable-graymap",  ["pgm"]);
    addType("image/x-portable-pixmap",  ["ppm"]);    
    addType("image/x-rgb",  ["rgb"]);
    addType("image/x-xbitmap",  ["xbm"]);
    addType("image/x-xpixmap",  ["xpm"]);
    addType("video/mpeg", ["mpeg","mpg","mpe"]);
    addType("video/mp4", ["mp4", "m4"]);
    
//    addType("text/javascript; charset=utf-8", ["js"]);
    addType("text/css", ["css"]);
    addRequestHeader("Server", "VIC 20");
    
}
