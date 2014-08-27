TEMPLATE = app
TARGET = strategy
INCLUDEPATH += .
CONFIG += c++11
CONFIG += debug_and_release

# CONFIG += soft
QT     -= core

*-g++* {
  QMAKE_CXXFLAGS += -pedantic-errors
}

# Input
HEADERS += unit.h storage.h transactor.h ientity.h istoragevalue.h storagevalue.h storageobject.h
SOURCES += main.cpp unit.cpp storage.cpp transactor.cpp ientity.cpp istoragevalue.cpp storagevalue.cpp storageobject.cpp
