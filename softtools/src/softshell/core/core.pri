isEmpty (core_pri_defined)
{

INCLUDEPATH += $$PWD $$(SOFTBASE)/include
DEPENDPATH  += $$PWD

include(../../plugins/common/common.pri)

HEADERS += $$PWD/scriptplugins.h \
           $$PWD/softshell.h     \
           $$PWD/protocall.h     \
	   $$PWD/automodule.h    

SOURCES += $$PWD/scriptplugins.cpp \
           $$PWD/softshell.cpp     \ 
	   $$PWD/automodule.cpp

core_pri_defined = true
}

