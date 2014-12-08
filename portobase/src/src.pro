TEMPLATE     = lib
VERSION      = 1.0.24
INCLUDEPATH += .
DEPENDPATH  += .
CONFIG      += debug_and_release
CONFIG      += c++11
CONFIG      += pretty
TARGET       = $$qtLibraryTarget(porto)
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

HEADERS += portons.h porto.h scriptengine.h
SOURCES += porto.cpp scriptengine.cpp

target.path = $$(PORTOBASE)/lib
headers.path = $$(PORTOBASE)/include
headers.files = $$HEADERS Porto
INSTALLS += headers target
