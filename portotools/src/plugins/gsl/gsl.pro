TEMPLATE        = lib
CONFIG         += plugin
INCLUDEPATH   += . $$(PORTOBASE)/include
DEPENDPATH    += . $$(PORTOBASE)/include
CONFIG        += debug_and_release
CONFIG        += c++11
CONFIG        += portomongo
CONFIG        += pretty
CONFIG        += gsl
CONFIG        += doxygen
QT            += script
TARGET         = $$qtLibraryTarget(gslplugin)

*-g++* {
  QMAKE_CXXFLAGS += -pedantic-errors
}

CONFIG(debug, debug|release){
  DESTDIR   = debug
} else {
  DESTDIR = release
  DEFINES += QT_NO_DEBUG_OUTPUT
  DEFINES += QT_NO_DEBUG
}

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR     = $$DESTDIR/.moc
RCC_DIR     = $$DESTDIR/.rcc
UI_DIR      = $$DESTDIR/.ui

include(../common/common.pri)

include(gslsource.pri)
HEADERS += gslplugin.h 
SOURCES += gslplugin.cpp 

DOXYPATH = $$PWD/doc

# Install
target.path = $$(PORTOBASE)/bin/plugins
INSTALLS   += target

