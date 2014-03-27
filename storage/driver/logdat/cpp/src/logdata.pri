isEmpty(porto_storage_logdata_defined) {

DEPENDPATH  += $$PWD
INCLUDEPATH += $$PWD

include($$PWD/../../../common/cpp/src/common.pri)

HEADERS     += $$PWD/logdata.h \ 
               $$PWD/logdatareader.h \
	       $$PWD/logdataexternalstorage.h 

SOURCES     += $$PWD/logdatareader.cpp \
	       $$PWD/logdataexternalstorage.cpp

porto_storage_logdata_defined = true
}