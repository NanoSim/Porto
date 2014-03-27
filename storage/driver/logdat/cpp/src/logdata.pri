DEPENDPATH  += $$PWD
INCLUDEPATH += $$PWD
include($$PWD/../../common/c++/common.pri)

HEADERS     += $$PWD/logdata.h \ 
               $$PWD/logdatareader.h \
	       $$PWD/logdataexternalstorage.h 

SOURCES     += $$PWD/logdatareader.cpp \
	       $$PWD/logdataexternalstorage.cpp
