TEMPLATE     = app
TARGET       = softshell
INCLUDEPATH += . 
DEPENDPATH  += . resources/

CONFIG      -= window
CONFIG      += console
CONFIG      += c++11
CONFIG      += debug_and_release
CONFIG      += soft
CONFIG      += pretty

QT         += gui
QT         += script
QT         += network
QT         += scripttools
QT         += concurrent
QT         += widgets

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

include(core/core.pri)
include(utils/utils.pri)

SOURCES += main.cpp
RESOURCES += softshell.qrc

# Install
target.path = $$(SOFTBASE)/bin
INSTALLS   += target
