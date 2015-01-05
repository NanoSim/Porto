TEMPLATE     = lib
INCLUDEPATH += .
DEPENDPATH  += .
CONFIG      += c++11
CONFIG      += debug_and_release
CONFIG      += pretty
CONFIG      += hdf5

TARGET       = $$qtLibraryTarget(softh5)

include(../driver/driver.pri)
target.path   = $$(SOFTBASE)/lib
headers.path  = $$(SOFTBASE)/include
headers.files = $$HEADERS ../driver/QH5
prf.path      = $$(SOFTBASE)/prf
prf.files     = softh5.prf
INSTALLS += headers target prf
