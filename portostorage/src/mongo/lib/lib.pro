TEMPLATE     = lib
INCLUDEPATH += .
DEPENDPATH  += .
CONFIG      += c++11
CONFIG      += debug_and_release
CONFIG      += pretty
CONFIG      += porto
CONFIG      += mongo
CONFIG      += bson
TARGET       = $$qtLibraryTarget(portomongo)

include(../driver/driver.pri)
target.path   = $$(PORTOBASE)/lib
headers.path  = $$(PORTOBASE)/include
headers.files = $$HEADERS ../driver/QMongo
prf.path      = $$(PORTOBASE)/prf
prf.files     = portomongo.prf
INSTALLS += headers target prf
