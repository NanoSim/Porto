isEmpty (utils_ptr_defined)
{
INCLUDEPATH += $$PWD $$(PORTOBASE)/include
DEPENDPATH  += $$PWD

HEADERS += $$PWD/utils.h \
           $$PWD/procs.h \
           $$PWD/procsprototype.h \
	   $$PWD/application.h \
	   $$PWD/applicationprototype.h \
           $$PWD/eventloop.h \
	   $$PWD/eventloopprototype.h \
	   $$PWD/udpsocket.h \
	   $$PWD/udpsocketprototype.h \	
	   $$PWD/console.h \	  
	   $$PWD/filesystem.h \
	   $$PWD/filesystemwatcher.h \
	   $$PWD/filesystemwatcherprototype.h \
	   $$PWD/httpserver.h \
	   $$PWD/httpd.h \
	   $$PWD/httpdprototype.h \
	   $$PWD/hostinfo.h \
	   $$PWD/concurrent.h \
#	   $$PWD/memtest.h \
#	   $$PWD/memtestinterface.h \
#	   $$PWD/memtestprototype.h \
	   $$PWD/async.h \
	   $$PWD/asyncinterface.h \
	   $$PWD/asyncprototype.h \
	   $$PWD/base64.h

SOURCES += $$PWD/utils.cpp \	      
           $$PWD/procs.cpp \
           $$PWD/procsprototype.cpp \
	   $$PWD/application.cpp \
	   $$PWD/applicationprototype.cpp \
           $$PWD/eventloop.cpp \
	   $$PWD/eventloopprototype.cpp \
	   $$PWD/udpsocket.cpp \
	   $$PWD/udpsocketprototype.cpp \
	   $$PWD/console.cpp \
	   $$PWD/filesystem.cpp \
	   $$PWD/filesystemwatcher.cpp \
	   $$PWD/filesystemwatcherprototype.cpp \
	   $$PWD/httpserver.cpp \
	   $$PWD/httpd.cpp \
	   $$PWD/httpdprototype.cpp \
  	   $$PWD/hostinfo.cpp \
           $$PWD/concurrent.cpp \
#	   $$PWD/memtest.cpp \
#	   $$PWD/memtestinterface.cpp \
#	   $$PWD/memtestprototype.cpp \
	   $$PWD/async.cpp \
	   $$PWD/asyncinterface.cpp \
	   $$PWD/asyncprototype.cpp \
  	   $$PWD/base64.cpp


utils_ptr_defined = true
}