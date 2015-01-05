isEmpty (storage_pri_defined)
{

  INCLUDEPATH += $$PWD
  DEPENDPATH  += $$PWD

  HEADERS += $$PWD/storage.h \
             $$PWD/ientity.h \
             $$PWD/istoragevalue.h \
             $$PWD/storagevalue.h \
             $$PWD/storageobject.h \
             $$PWD/transactor.h

  SOURCES += $$PWD/storage.cpp \
             $$PWD/ientity.cpp \
             $$PWD/istoragevalue.cpp \
             $$PWD/storagevalue.cpp \
             $$PWD/storageobject.cpp \
             $$PWD/transactor.cpp

  storage_pri_defined = true
}