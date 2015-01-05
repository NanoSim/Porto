TEMPLATE = app
TARGET = simpler
INCLUDEPATH += .
CONFIG += soft
CONFIG += c++11
CONFIG += hdf5

# Input
SOURCES += main.cpp

include (../../../src/hdf5/driver/driver.pri)
