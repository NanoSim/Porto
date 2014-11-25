INCLUDEPATH += $$PWD
DEPENDPATH  += $$PWD

HEADERS += $$PWD/qh5.h \
           $$PWD/qh5file.h \
           $$PWD/qh5dataset.h \
	   $$PWD/qh5dataspace.h \
	   $$PWD/qh5datatype.h \
	   $$PWD/qh5group.h \
	   $$PWD/qh5property.h 

SOURCES += $$PWD/qh5.cpp \
           $$PWD/qh5file.cpp \
           $$PWD/qh5dataset.cpp \
	   $$PWD/qh5dataspace.cpp \
	   $$PWD/qh5group.cpp \
	   $$PWD/qh5property.cpp