TEMPLATE     = app
TARGET       = logdatatest
INCLUDEPATH += .
DEPENDPATH  += .
CONFIG      += console
CONFIG      += gtest
CONFIG      += debug_and_release
CONFIG      += c++11

CONFIG(debug, debug|release){
  DESTDIR = debug
} else {
  DESTDIR = release
}

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR     = $$DESTDIR/.moc
RCC_DIR     = $$DESTDIR/.rcc
UI_DIR      = $$DESTDIR/.ui

include(../src/logdata.pri)

# Input
SOURCES += logdattest.cpp

win32:QMAKE_POST_LINK = $(DESTDIR_TARGET)
unix:QMAKE_POST_LINK  = ./$(TARGET)
