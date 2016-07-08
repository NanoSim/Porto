#!/usr/bin/env softshell

__main__ = function (args)
{
    var eventLoop = new EventLoop();
    var networkAccess = new NetworkAccessManager();

    networkAccess["finished(QNetworkReply*)"].connect(function (reply){
	print ("finished\n")
	print (reply.readAll());
    });
    networkAccess.get("http://localhost:8080/metadata/mesh-0.1-SNAPSHOT2.json");   
    eventLoop.exec();
}
