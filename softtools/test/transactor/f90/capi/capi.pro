TEMPLATE = app
TARGET = capi
INCLUDEPATH += . ../../wrapper
LIBS += -L ../../wrapper
LIBS += -lwrapper


# Input
SOURCES += main.cpp
