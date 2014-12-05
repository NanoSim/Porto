__main__ = function(args)
{
    var event = new EventLoop();

    var fsw = new FileSystemWatcher();
    fsw.addPath("/tmp/test.txt");
    fsw["fileChanged(QString)"].connect(function(file){
	print ("file",file,"did change");
    });	

    event.exec();
}
