isEmpty(porto_storage_common_defined) {

DEPENDPATH   += $$PWD
INCLUDEPATH  += $$PWD

HEADERS      += $$PWD/iexternalstoragedriver.h \ 
                $$PWD/externalstorage.h 

SOURCES      += $$PWD/iexternalstoragedriver.cpp \
                $$PWD/externalstorage.cpp

porto_storage_common_defined = true
}