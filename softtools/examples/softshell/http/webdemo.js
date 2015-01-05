__main__ = function (args)
{
    var event = new EventLoop(),
        port  = 8081,
        httpd = new HttpServer(port);

    httpd.setRootDir("html/");
    httpd.start();
    event.exec();
}

