isEmpty(soft_storage_logdata_defined) {
  load(soft_paths)

  DEPENDPATH  += $$PWD
  INCLUDEPATH += $$PWD

  include($$SOFT_STORAGE_EXTERNAL/src/common/common.pri)

  HEADERS     += $$PWD/logdata.h \ 
               $$PWD/logdatareader.h \
	       $$PWD/logdataexternalstorage.h 

  SOURCES     += $$PWD/logdatareader.cpp \
	       $$PWD/logdataexternalstorage.cpp

  soft_storage_logdata_defined = true
}