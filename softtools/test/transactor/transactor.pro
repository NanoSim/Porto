TEMPLATE = app

# Config
CONFIG += c++11
CONFIG += console
CONFIG += mongo bson

QT     -= gui
TARGET = transactor

# Project deps
INCLUDEPATH += .

# Input
SOURCES += main.cpp           \
           qmongoclient.cpp   \
	   qmongocollection.cpp \
	   qmongocursor.cpp   \
	   qbson.cpp          \
	   qbsoniter.cpp      \
	   qbsonvalue.cpp     \
	   entity.cpp         \
	   pipe.cpp           \
	   transactor.cpp     \
	   datastream.cpp     \
	   storagestrategy.cpp\
	   mongostrategy.cpp
	   
HEADERS += qmongoclient.h     \
           qmongocollection.h \
	   qmongocursor.h     \
	   qbson.h            \
	   qbsoniter.h        \
	   qbsonvalue.h       \
	   entity.h           \
	   pipe.h             \
	   transactor.h       \
	   datastream.h       \
	   storagestrategy.h  \
	   mongostrategy.h
		   
