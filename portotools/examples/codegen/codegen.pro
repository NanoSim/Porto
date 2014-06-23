entitygencxx.output   = ${QMAKE_FILE_BASE}.cpp
entitygencxx.commands = $$PWD/gencxx.js ${QMAKE_FILE_BASE} > ${QMAKE_FILE_OUT}
entitygencxx.input    = ENTITY
entitygencxx.depends  = ${QMAKE_FILE_BASE}.h
entitygencxx.variable_out = GENERATED_SOURCES
entitygencxx.dependency_type = TYPE_C
entitygencxx.name = "entity build"

entitygenhxx.output   = ${QMAKE_FILE_BASE}.h
entitygenhxx.commands = $$PWD/genhxx.js ${QMAKE_FILE_BASE} > ${QMAKE_FILE_OUT}
entitygenhxx.input    = ENTITY
entitygenhxx.variable_out = HEADERS
entitygenhxx.dependency_type = TYPE_C
entitygenhxx.name = "entity header"

QMAKE_EXTRA_COMPILERS += entitygenhxx entitygencxx

TEMPLATE = app
TARGET = codegen
INCLUDEPATH += .

# Input
ENTITY  += unit.json
HEADERS += storage.h ientity.h istoragevalue.h storagevalue.h storageobject.h transactor.h
SOURCES += main.cpp storage.cpp ientity.cpp istoragevalue.cpp storagevalue.cpp storageobject.cpp transactor.cpp
