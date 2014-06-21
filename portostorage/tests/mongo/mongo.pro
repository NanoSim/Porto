TEMPLATE    = app
CONFIG     += gtest console c++11 bson mongo debug_and_release
CONFIG     -= window
# CONFIG     += valgrind
QT         -= gui
*-g++* {
  QMAKE_CXXFLAGS += -pedantic-errors
}

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

include(../../src/mongo/driver/driver.pri)

SOURCES += mongotest.cpp

win32:QMAKE_POST_LINK = $(DESTDIR_TARGET) --gtest_output=xml:$$PWD/result.xml
contains(CONFIG, valgrind) {
  unix:QMAKE_POST_LINK = valgrind --tool=memcheck --leak-check=yes  --gen-suppressions=all ./$(TARGET) --gtest_output=xml:$$PWD/result.xml
} else {
  unix:QMAKE_POST_LINK = ./$(TARGET)  --gtest_output=xml:$$PWD/result.xml
}

