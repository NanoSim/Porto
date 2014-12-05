isEmpty(porto_storage_logdata_defined) {
  load(porto_paths)

  DEPENDPATH  += $$PWD
  INCLUDEPATH += $$PWD

  include($$PORTO_STORAGE_EXTERNAL/src/common/common.pri)

  HEADERS     += $$PWD/logdata.h \ 
               $$PWD/logdatareader.h \
	       $$PWD/logdataexternalstorage.h 

  SOURCES     += $$PWD/logdatareader.cpp \
	       $$PWD/logdataexternalstorage.cpp

  porto_storage_logdata_defined = true
}