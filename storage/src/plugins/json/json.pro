TEMPLATE       = lib
CONFIG        += plugin
CONFIG        += soft-core-base
CONFIG        += c++11
CONFIG        += bson
CONFIG        += mongo
TARGET         = $$qtLibraryTarget(storage-plugin-json)
QT            -= gui

*-g++* {
  QMAKE_CXXFLAGS += -pedantic-errors
}

CONFIG(debug, debug|release){
  DESTDIR    = debug
} else {
  DESTDIR = release
#  DEFINES += QT_NO_DEBUG_OUTPUT
#  DEFINES += QT_NO_DEBUG
}

OBJECTS_DIR  = $$DESTDIR/.obj
MOC_DIR      = $$DESTDIR/.moc
RCC_DIR      = $$DESTDIR/.rcc
UI_DIR       = $$DESTDIR/.ui

include ($$PWD/jsonstrategyplugin.pri)
SOURCES += jsonstrategyplugin.cpp
HEADERS += jsonstrategyplugin.h

target.path = $$(SOFTBASE)/bin/plugins
INSTALLS += target              
