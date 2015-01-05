TEMPLATE     = lib
INCLUDEPATH += .
DEPENDPATH  += .
CONFIG      += c++11
CONFIG      += debug_and_release
CONFIG      += pretty
CONFIG      += soft
CONFIG      += mongo
CONFIG      += bson
TARGET       = $$qtLibraryTarget(softmongo)

include(../driver/driver.pri)
target.path   = $$(SOFTBASE)/lib
headers.path  = $$(SOFTBASE)/include
headers.files = $$HEADERS ../driver/QMongo
prf.path      = $$(SOFTBASE)/prf
prf.files     = softmongo.prf
INSTALLS += headers target prf
