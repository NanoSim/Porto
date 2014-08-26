TEMPLATE     = lib
VERSION      = 0.1
INCLUDEPATH += .
DEPENDPATH  += .
CONFIG      += debug_and_release
CONFIG      += c++11
CONFIG      += pretty
TARGET       = $$qtLibraryTarget(soft)
QT          += script

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

HEADERS += softns.h soft.h scriptengine.h
SOURCES += soft.cpp scriptengine.cpp

target.path = $$(SOFTBASE)/lib
headers.path = $$(SOFTBASE)/include
headers.files = $$HEADERS Soft
INSTALLS += headers target
