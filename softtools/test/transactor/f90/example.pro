TEMPLATE = app
CONFIG  += ifort
QMAKE_LINK = ifort
MODULE_DIR = .

F90SOURCES += istoragestrategy.f90 \
              mongostrategy.f90 \              
              storagefactory.f90 \
              transactor.f90 \
              main.f90
