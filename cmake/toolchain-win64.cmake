# A toolchain file to cross compiling for win64 configured for
# bamboo.code.sintef.no. For other systems, make a private copy of
# this file.
# 
# To use it, do:
#     $ ccmake -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain-win64.cmake ..
#
# For more info, see http://www.vtk.org/Wiki/CmakeMingw.
#


# target we want to compile for
set(CROSS_TARGET "win64"
  CACHE STRING "Cross-compiling target. E.g. 'win32' or 'win64'")


# the name of the target operating system
set(CMAKE_SYSTEM_NAME Windows)


# set COMPILER_PREFIX, see http://www.mingw.org/
set(COMPILER_PREFIX "x86_64-w64-mingw32-")


# which compilers to use
find_program(CMAKE_RC_COMPILER NAMES ${COMPILER_PREFIX}windres)
find_program(CMAKE_C_COMPILER NAMES ${COMPILER_PREFIX}gcc)
find_program(CMAKE_CXX_COMPILER NAMES ${COMPILER_PREFIX}g++)
find_program(CMAKE_Fortran_COMPILER NAMES ${COMPILER_PREFIX}gfortran)

# wine executable
find_program(WINE NAMES wine64)
set(RUNNER ${WINE})


# here is the target environment located
set(CMAKE_FIND_ROOT_PATH /usr/${COMPILER_PREFIX} ${CMAKE_INSTALL_PREFIX})

# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search 
# programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

