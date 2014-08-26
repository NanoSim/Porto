isEmpty(mongo_driver_defined) {

DEPENDPATH  += $$PWD $$(SOFTBASE)/include
INCLUDEPATH += $$PWD $$(SOFTBASE)/include

HEADERS  += $$PWD/mongons.h                 \
	    $$PWD/qmongoinitialize.h        \
            $$PWD/qmongoclient.h            \
	    $$PWD/qmongoclientpool.h        \
            $$PWD/qmongocollection.h        \
	    $$PWD/qmongocursor.h            \
	    $$PWD/qmongodatabase.h          \
	    $$PWD/qmongogridfs.h            \
	    $$PWD/qmongogridfsfile.h        \
	    $$PWD/qmongogridfsfilelist.h    \
	    $$PWD/qmongogridfsfileopt.h     \
	    $$PWD/qmongoreadprefs.h         \
	    $$PWD/qmongostream.h            \
	    $$PWD/qmongouri.h               \
	    $$PWD/qmongowriteconcern.h      \
	    $$PWD/qmongoqueryflags.h        \
	    $$PWD/qbson.h

SOURCES  += $$PWD/qmongoinitialize.cpp      \
            $$PWD/qmongoclient.cpp          \
	    $$PWD/qmongoclientpool.cpp      \
            $$PWD/qmongocollection.cpp      \
	    $$PWD/qmongocursor.cpp          \
	    $$PWD/qmongodatabase.cpp        \
	    $$PWD/qmongogridfs.cpp          \
	    $$PWD/qmongogridfsfile.cpp      \
	    $$PWD/qmongogridfsfilelist.cpp  \
	    $$PWD/qmongogridfsfileopt.cpp   \
	    $$PWD/qmongoreadprefs.cpp       \
	    $$PWD/qmongostream.cpp          \
	    $$PWD/qmongouri.cpp             \
	    $$PWD/qmongowriteconcern.cpp    \
	    $$PWD/qmongoqueryflags.cpp	    \	
	    $$PWD/qbson.cpp


mongo_driver_defined = true
}
