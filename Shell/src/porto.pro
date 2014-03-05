TEMPLATE     = app
TARGET       = porto
DEPENDPATH  += . ./res custom
INCLUDEPATH += .
CONFIG      -= windows
CONFIG      += console
CONFIG      += c++11
CONFIG      += debug_and_release
QT          -= gui
QT          += script
QT          += concurrent


debug:DESTDIR   = debug
release:DESTDIR = release

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR     = $$DESTDIR/.moc
RCC_DIR     = $$DESTDIR/.rcc
UI_DIR      = $$DESTDIR/.ui

# Input
SOURCES += main.cpp sys.cpp
HEADERS += sys.h
RESOURCES += porto.qrc

# install
target.path = ../bin
INSTALLS   += target
