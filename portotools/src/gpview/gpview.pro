TEMPLATE = app
TARGET = gpview
INCLUDEPATH += .
CONFIG += c++11

QT += widgets
QT += gui

QMAKE_CXXFLAGS += -pedantic-errors

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

# Input
SOURCES += main.cpp

# Install
target.path = $$(PORTOBASE)/bin
INSTALLS   += target
