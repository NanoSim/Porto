TEMPLATE     = app
TARGET       = codegen
INCLUDEPATH += .
DEPENDPATH  += .
CONFIG      += c++11

include('storage/storage.pri')

*-g++* {
  QMAKE_CXXFLAGS += -pedantic-errors
}

OBJECTS_DIR = .obj
MOC_DIR     = .moc
RCC_DIR     = .rcc
UI_DIR      = .ui

# MVC View sources
ENTITY_CPP_VIEW = entity.cxxjs
ENTITY_H_VIEW   = entity.hxxjs

# Meta-Data 
ENTITY  += unit.json example.json

# Input Sources
SOURCES += main.cpp

# Code Generators
entitygencxx.name     = "entity build"
entitygencxx.output   = ${QMAKE_FILE_BASE}.cpp
entitygencxx.commands = $$PWD/genxx.js ${QMAKE_FILE_BASE} $$ENTITY_CPP_VIEW > ${QMAKE_FILE_OUT}
entitygencxx.input    = ENTITY
entitygencxx.depends  = ${QMAKE_FILE_BASE}.h 
entitygencxx.variable_out = SOURCES
entitygencxx.dependency_type = TYPE_C
 
entitygenhxx.name     = "entity header"
entitygenhxx.output   = ${QMAKE_FILE_BASE}.h
entitygenhxx.commands = $$PWD/genxx.js ${QMAKE_FILE_BASE} $$ENTITY_H_VIEW > ${QMAKE_FILE_OUT}
entitygenhxx.input    = ENTITY
entitygenhxx.depends  = $$ENTITY $$ENTITY_H_VIEW $$ENTITY_CPP_VIEW
entitygenhxx.variable_out = HEADERS
entitygenhxx.dependency_type = TYPE_C

QMAKE_EXTRA_COMPILERS += entitygenhxx entitygencxx
