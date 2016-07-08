option (WITH_C "Enable C API" ON)
option (WITH_FORTRAN "Enable Fortran API" OFF)

if (WITH_C)
  add_subdirectory(c/src/softc)
endif (WITH_C)

if (WITH_FORTRAN)
  add_subdirectory(fortran/src/softf)
endif (WITH_FORTRAN)
