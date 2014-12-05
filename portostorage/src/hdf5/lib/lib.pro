TEMPLATE     = lib
INCLUDEPATH += .
DEPENDPATH  += .
CONFIG      += c++11
CONFIG      += debug_and_release
CONFIG      += pretty
CONFIG      += hdf5

TARGET       = $$qtLibraryTarget(portoh5)

include(../driver/driver.pri)
target.path   = $$(PORTOBASE)/lib
headers.path  = $$(PORTOBASE)/include
headers.files = $$HEADERS ../driver/QH5
prf.path      = $$(PORTOBASE)/prf
prf.files     = portoh5.prf
INSTALLS += headers target prf
