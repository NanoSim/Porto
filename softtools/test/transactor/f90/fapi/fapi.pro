TEMPLATE   = app
CONFIG    += ifort
CONFIG    += debug_and_release
QMAKE_LINK = ifort
MODULE_DIR = .
QT        -= gui core

CONFIG(debug, debug|release){
  DESTDIR   = debug
} else {
  DESTDIR = release
  DEFINES += QT_NO_DEBUG_OUTPUT
  DEFINES += QT_NO_DEBUG
}

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR     = $$DESTDIR/.moc
RCC_DIR     = $$DESTDIR/.rcc
UI_DIR      = $$DESTDIR/.ui

INCLUDEPATH += . ../../wrapper
LIBS += -L ../../wrapper
LIBS += -lwrapper


F90SOURCES += softf_transactor.f90 \
              test.f90
# Directories
