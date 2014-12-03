TEMPLATE       = lib
CONFIG        += plugin
INCLUDEPATH   += . $$(SOFTBASE)/include
DEPENDPATH    += . $$(SOFTBASE)/include
CONFIG        += debug_and_release
CONFIG        += c++11
CONFIG        += softmongo
CONFIG        += pretty
QT            += script
TARGET         = $$qtLibraryTarget(mongoplugin)

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
include(mongoprototypes.pri)

# Input
HEADERS += mongoplugin.h 
SOURCES += mongoplugin.cpp 

# Install
target.path = $$(SOFTBASE)/bin/plugins
INSTALLS   += target
