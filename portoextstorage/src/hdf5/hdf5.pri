isEmpty(porto_storage_hdf5_defined) {

DEPENDPATH  += $$PWD
INCLUDEPATH += $$PWD

include($${PORTO_STORAGE_EXTERNAL}/src/common/common.pri)

HEADERS     += $$PWD/hdf5.h \ 
               $$PWD/hdf5reader.h \
	       $$PWD/hdf5externalstorage.h 

SOURCES     += $$PWD/hdf5reader.cpp \
	       $$PWD/hdf5externalstorage.cpp

porto_storage_hdf5_defined = true
}