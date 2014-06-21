isEmpty (core_pri_defined)
{

INCLUDEPATH += $$PWD $$(PORTOBASE)/include
DEPENDPATH  += $$PWD

include(../../plugins/common/common.pri)

HEADERS += $$PWD/scriptplugins.h \
           $$PWD/portoshell.h     \
           $$PWD/protocall.h     \
	   $$PWD/automodule.h    

SOURCES += $$PWD/scriptplugins.cpp \
           $$PWD/portoshell.cpp     \ 
	   $$PWD/automodule.cpp

core_pri_defined = true
}

