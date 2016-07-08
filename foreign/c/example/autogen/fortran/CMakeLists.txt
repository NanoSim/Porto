project (testlib-f90)
enable_language (Fortran)

# default installation
get_filename_component (default_prefix ".." ABSOLUTE)
set (CMAKE_INSTALL_PREFIX ${default_prefix} CACHE STRING
      "Choose the installation directory; by default it installs in the NORMA directory."
      FORCE)

# FFLAGS depend on the compiler
get_filename_component (Fortran_COMPILER_NAME ${CMAKE_Fortran_COMPILER} NAME)

if (Fortran_COMPILER_NAME MATCHES "gfortran.*")
  # gfortran
  set (CMAKE_Fortran_FLAGS_RELEASE "-funroll-all-loops -fno-f2c -O3")
  set (CMAKE_Fortran_FLAGS_DEBUG   "-fno-f2c -O0 -g")
elseif (Fortran_COMPILER_NAME MATCHES "ifort.*")
  # ifort (untested)
  set (CMAKE_Fortran_FLAGS_RELEASE "-f77rtl -O3")
  set (CMAKE_Fortran_FLAGS_DEBUG   "-f77rtl -O0 -g")
elseif (Fortran_COMPILER_NAME MATCHES "g77")
  # g77
  set (CMAKE_Fortran_FLAGS_RELEASE "-funroll-all-loops -fno-f2c -O3 -m32")
  set (CMAKE_Fortran_FLAGS_DEBUG   "-fno-f2c -O0 -g -m32")
else (Fortran_COMPILER_NAME MATCHES "gfortran.*")
#  message ("CMAKE_Fortran_COMPILER full path: " ${CMAKE_Fortran_COMPILER})
#  message ("Fortran compiler: " ${Fortran_COMPILER_NAME})
#  message ("No optimized Fortran compiler flags are known, we just try -O2...")
  set (CMAKE_Fortran_FLAGS_RELEASE "-O2")
  set (CMAKE_Fortran_FLAGS_DEBUG   "-O0 -g")
endif (Fortran_COMPILER_NAME MATCHES "gfortran.*")

set (LINK_LIBS ${LINK_LIBS})

include_directories(${CMAKE_CURRENT_BINARY_DIR}/../entities/)
message(STATUS ${F90_SRC})
add_library (${PROJECT_NAME} SHARED "softf-allocatable.f90" "sim-handle.f90" "${example-autogen_BINARY_DIR}/entities/simple.f90" "user-ctx.f90" "demo.f90")
target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})
set_source_files_properties("${example-autogen_BINARY_DIR}/entities/simple.f90"
  PROPERTIES GENERATED 1)

set(${PROJECT_NAME}_LIBS
  ${PROJECT_NAME}
  ${LINK_LIBS}
  CACHE INTERNAL "${PROJECT_NAME}: Link Libs" FORCE)

set(${PROJECT_NAME}_INCLUDE_DIRS
  ${PROJECT_SOURCE_DIR}
  CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
