# Config
TEMPLATE       = lib
TARGET         = softc
INCLUDEPATH   += .
QT            -= core gui

# Input
HEADERS       += softc-macros.h \
                 softc-transactor.h \
                 softc-transactor-private.h \
                 softc-utils.h
	   
SOURCES       += softc-transactor.c \
                 softc-utils.c

# Installs
target.path   = $$(SOFTBASE)/lib
                                 
headers.path  = $$(SOFTBASE)/include
headers.files = softc-macros.h \
                softc-transactor.h \
                softc-utils.h
		
prf.path      = $$(SOFTBASE)/prf
prf.files     = softc.prf

INSTALLS     += target headers prf
