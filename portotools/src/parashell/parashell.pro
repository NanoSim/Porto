TEMPLATE     = app
TARGET       = parashell
CONFIG      += c++11
CONFIG      += console
INCLUDEPATH += .
QT          += script network

# Input
SOURCES += main.cpp    \
           console.cpp \
	   prompt.cpp  
	 

HEADERS += console.h   \
           prompt.h    \
	   protocall.h \

include("utils/utils.pri")