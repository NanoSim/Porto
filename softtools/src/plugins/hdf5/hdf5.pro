TEMPLATE       = lib
CONFIG        += plugin
INCLUDEPATH   += . $$(SOFTBASE)/include
DEPENDPATH    += . $$(SOFTBASE)/include
CONFIG        += debug_and_release
CONFIG        += c++11
CONFIG        += hdf5
CONFIG        += pretty
CONFIG        += soft
QT            += script
TARGET         = $$qtLibraryTarget(h5plugin)

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
include($$(SOFTBASE)/../softstorage/src/hdf5/driver/driver.pri)
include(h5prototypes.pri)

# Input
HEADERS += h5plugin.h
SOURCES += h5plugin.cpp
           

# Install
target.path = $$(SOFTBASE)/bin/plugins
INSTALLS   += target

