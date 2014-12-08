isEmpty (h5driver_defined) {

INCLUDEPATH += $$PWD $$(PORTOBASE)/include
DEPENDPATH  += $$PWD $$(PORTOBASE)/include

HEADERS += $$PWD/h5ns.h \
           $$PWD/qh5.h \
           $$PWD/qh5file.h \
           $$PWD/qh5dataset.h \
	   $$PWD/qh5dataspace.h \
	   $$PWD/qh5datatype.h \
	   $$PWD/qh5group.h \
	   $$PWD/qh5property.h \
	   $$PWD/qh5attribute.h

SOURCES += $$PWD/qh5.cpp \
           $$PWD/qh5file.cpp \
           $$PWD/qh5dataset.cpp \
	   $$PWD/qh5dataspace.cpp \
	   $$PWD/qh5datatype.cpp \
	   $$PWD/qh5group.cpp \
	   $$PWD/qh5property.cpp \
	   $$PWD/qh5attribute.cpp

h5driver_defined = true
}