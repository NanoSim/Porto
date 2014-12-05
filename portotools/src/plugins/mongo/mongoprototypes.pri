INCLUDEPATH   += $$(PORTOBASE)/include $$PWD
DEPENDPATH    += $$(PORTOBASE)/include $$PWD

# Input
HEADERS += $$PWD/mongoclientprototype.h \
           $$PWD/mongocollectionprototype.h \
           $$PWD/mongodatabaseprototype.h \
           $$PWD/mongouriprototype.h \
           $$PWD/mongoreadprefsprototype.h \
           $$PWD/mongocursorprototype.h \
           $$PWD/bsonprototype.h

SOURCES += $$PWD/mongoclientprototype.cpp \
           $$PWD/mongocollectionprototype.cpp \
           $$PWD/mongodatabaseprototype.cpp \
           $$PWD/mongouriprototype.cpp \
           $$PWD/mongoreadprefsprototype.cpp \
           $$PWD/mongocursorprototype.cpp \
           $$PWD/bsonprototype.cpp
