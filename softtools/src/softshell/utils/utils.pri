isEmpty (utils_ptr_defined)
{
INCLUDEPATH += $$PWD $$(SOFTBASE)/include
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
	   $$PWD/hostinfo.h 


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
  	   $$PWD/hostinfo.cpp 


utils_ptr_defined = true
}