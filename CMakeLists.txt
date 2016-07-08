cmake_minimum_required(VERSION 3.2)
include(CPack)
project(soft5)

option (WITH_STORAGE_MONGO "MongoDB storage support"      ON)
option (WITH_STORAGE_JSON "JSON storage support"          ON)
option (WITH_STORAGE_EXTERNAL "External storage support"  ON)
option (WITH_PLUGIN_SOFT_KERNEL "Compile kernel plugin"   ON)
option (WITH_FOREIGN_C "Compile with C API"               ON)
option (WITH_TESTS "Compile tests"                        ON)
option (WITH_WEB "Compile with Web support"               OFF)
option (WITH_HSM "Compile with HSM plugin"                OFF)
option (WITH_STORAGE_HD5 "HDF5 storage support"           OFF)
option (WITH_PLUGIN_GSL "Compile GSL plugin"              OFF)
option (WITH_PLUGIN_NLOPT "Compile NLOpt plugin"          OFF)
option (WITH_PLUGIN_HD5 "Compile HDF5 plugin"             OFF)
option (WITH_PLUGIN_MONGO "Compile MongoDB plugin"        OFF)
option (WITH_FOREIGN_FORTRAN "Compile with Fortran API"   OFF)
option (WITH_FOREIGN_PYTHON "Compile with Python API"     OFF)
option (WITH_NUMERICS "Compile with SOFT Numerics"        OFF)
option (USE_PYTHON3 "Create Python 3 bindings instead of Python 2" OFF)
option (WITH_PORTO "Compile porto plugins"                OFF)

set (SOFT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Debug' as none was specified.")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions("-DQT_NO_DEBUG_OUTPUT")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
find_package (BSON REQUIRED)
find_package (Qt5Core REQUIRED)
find_package (Qt5Network REQUIRED)
find_package (Qt5Script REQUIRED)
find_package (Qt5Concurrent REQUIRED)

set (CMAKE_THREAD_PREFER_PTHREAD  TRUE)
set (CMAKE_INCLUDE_CURRENT_DIR    ON)
set (CMAKE_AUTOMOC                ON)
set (SOFT_ROOTDIR ${CMAKE_SOURCE_DIR})

if (WITH_PLUGIN_HD5 OR WITH_STORAGE_HD5)
  find_package(HDF5 REQUIRED)
endif ()

if (WITH_STORAGE_MONGO OR WITH_PLUGIN_MONGO)
  find_package(MONGOC REQUIRED)
endif()

set (SOFT_MAJOR_VERSION 5)
set (SOFT_MINOR_VERSION 1)
set (SOFT_PATCH_VERSION 5)

# Tests
# Run them with "ctest -V" or "make check" to get verbose output.
if (WITH_TESTS)
  enable_testing()
  add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --verbose -F)

endif (WITH_TESTS)

# Define content of PATH (Windows), LD_LIBRARY_PATH (Unix) and SOFTSTORAGE
# environment variables needed for running the tests without installation.
set (library_path
  ${soft-kernel_BINARY_DIR}
  ${soft-storage-hdf5_BINARY_DIR}
  ${soft-storage-mongo_BINARY_DIR}
  ${soft-storage-plugins-hdf5_BINARY_DIR}
  ${soft-storage-plugins-json_BINARY_DIR}
  ${soft-storage-plugins-mongo_BINARY_DIR}
  ${soft-numerics-sim_BINARY_DIR}
  ${softc_BINARY_DIR}
)
set (storage_path
  ${soft-storage-plugins-hdf5_BINARY_DIR}
  ${soft-storage-plugins-json_BINARY_DIR}
  ${soft-storage-plugins-mongo_BINARY_DIR}
)
if (UNIX)
  string(REPLACE ";" ":" LD_LIBRARY_PATH "${library_path}")
else ()
  set(PATH "${library_path}")
endif()
string(REPLACE ";" ":" SOFTSTORAGE "${storage_path}")

# Make sure that we install 64-bit libraries in lib64 on systems that
# use this convention
get_property(LIB64 GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS)
if ("${LIB64}" STREQUAL "TRUE")
  set(LIBSUFFIX 64)
else()
  set(LIBSUFFIX "")
endif()
set(LIB_INSTALL_DIR lib${LIBSUFFIX} CACHE PATH
  "Installation directory for libraries")
mark_as_advanced(LIB_INSTALL_DIR)

# Install targets
set (SOFTBASE ${LIB_INSTALL_DIR}/${PROJECT_NAME})
set (STORAGE_PLUGINS_INSTALL_DIR ${SOFTBASE}/storage)
set (SCRIPT_MODULES_INSTALL_DIR ${SOFTBASE}/modules)
set (SCRIPT_PLUGINS_INSTALL_DIR ${SOFTBASE}/plugins)
set (SIMUDF_PLUGINS_DIR ${SOFTBASE}/udf)
set (INCLUDE_INSTALL_DIR include/${PROJECT_NAME})

#####################
# add_forge
#####################

include(cmake/add_forge.cmake)


######################
# Add subdirectories
######################

# Core library
add_subdirectory(core/src/kernel)

if (WITH_FOREIGN_C)
  add_subdirectory(foreign/c/src/softc)
endif (WITH_FOREIGN_C)

add_subdirectory(tools/src/softshell)
add_subdirectory(tools/src/forge)

# Storage
if (WITH_STORAGE_HD5)
  add_subdirectory(storage/src/hdf5)
  add_subdirectory(storage/src/plugins/hdf5)
  if (WITH_TESTS)
    add_subdirectory(storage/test/hdf5)
    add_subdirectory(storage/test/plugins/hdf5)
  endif (WITH_TESTS)
endif (WITH_STORAGE_HD5)

if (WITH_STORAGE_MONGO)
  add_subdirectory(storage/src/mongo)
  add_subdirectory(storage/src/plugins/mongo)
  add_subdirectory(storage/src/plugins/mongo2)
  if (WITH_TESTS)
    add_subdirectory(storage/test/mongo)
    add_subdirectory(storage/test/plugins/mongo)
    add_subdirectory(storage/test/plugins/mongo2)
  endif(WITH_TESTS)
endif (WITH_STORAGE_MONGO)

if (WITH_STORAGE_JSON)
  add_subdirectory(storage/src/plugins/json)
endif (WITH_STORAGE_JSON)

if (WITH_STORAGE_EXTERNAL)
  add_subdirectory(storage/src/plugins/external)
  if (WITH_TESTS)
    add_subdirectory(storage/test/plugins/external)
  endif (WITH_TESTS)
endif (WITH_STORAGE_EXTERNAL)

# Tools
if (WITH_PLUGIN_MONGO)
  add_subdirectory(tools/src/plugins/mongo)
endif (WITH_PLUGIN_MONGO)


if (WITH_PLUGIN_GSL)
  add_subdirectory(tools/src/plugins/gsl)
endif(WITH_PLUGIN_GSL)

if (WITH_PLUGIN_NLOPT)
  add_subdirectory(tools/src/plugins/nlopt)
endif(WITH_PLUGIN_NLOPT)

if (WITH_PLUGIN_HD5)
  add_subdirectory(tools/src/plugins/hdf5)
endif(WITH_PLUGIN_HD5)

if (WITH_PLUGIN_SOFT_KERNEL)
  add_subdirectory(tools/src/plugins/soft-kernel)
  if (WITH_TESTS)
    add_subdirectory(tools/test/plugins/soft-kernel)
  endif (WITH_TESTS)
endif(WITH_PLUGIN_SOFT_KERNEL)

if (WITH_HSM)
  add_subdirectory(tools/src/plugins/hsm)
endif(WITH_HSM)

if (WITH_WEB)
  add_subdirectory(tools/src/web)
endif(WITH_WEB)


# Foreign
if (WITH_FOREIGN_C)
  if (WITH_TESTS)
    add_subdirectory(foreign/c/test/softc)
    add_subdirectory(foreign/c/test/storage)
  endif (WITH_TESTS)
endif (WITH_FOREIGN_C)

if (WITH_FOREIGN_FORTRAN)
  add_subdirectory(foreign/fortran/src/softf)
endif (WITH_FOREIGN_FORTRAN)

if (WITH_FOREIGN_PYTHON)
  if (USE_PYTHON3)
    set(Python_ADDITIONAL_VERSIONS 3.6 3.5 3.4 3.3 3.2)
  else (USE_PYTHON3)
    set(Python_ADDITIONAL_VERSIONS 2.7)
  endif (USE_PYTHON3)
  add_subdirectory(foreign/python/src/softpy)
  if (WITH_TESTS)
    add_subdirectory(foreign/python/test/softpy)
  endif (WITH_TESTS)
endif (WITH_FOREIGN_PYTHON)

if (WITH_NUMERICS)
  add_subdirectory(numerics/src/sim)
  if (WITH_TESTS)
    add_subdirectory(numerics/test/sim)
  endif(WITH_TESTS)
endif (WITH_NUMERICS)

if (WITH_PORTO)
  set (ENTITY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/porto/src/entities)
  set (C_SOURCE_TEMPLATE ${CMAKE_CURRENT_SOURCE_DIR}/porto/src/templates/ccode.cjs)
  set (C_HEADER_TEMPLATE ${CMAKE_CURRENT_SOURCE_DIR}/porto/src/templates/ccode.hjs)

  add_subdirectory(porto/src/entities)
  add_subdirectory(porto/src/thirdParty/chemkinReader)
  add_subdirectory(porto/src/thirdParty/remarcReader)
  add_subdirectory(porto/src/thirdParty/xyReader)
  add_subdirectory(porto/test/codegen)
  add_subdirectory(porto/src/json)
  add_subdirectory(porto/src/chemkin)
  add_subdirectory(porto/src/remarkinit)
  add_subdirectory(porto/src/xyplot)
  add_subdirectory(porto/src/dft-prepare)
  add_subdirectory(porto/src/remark-wrapper)

 if (WITH_TESTS)
   add_subdirectory(porto/test/json)
   add_subdirectory(porto/test/chemkin)
   add_subdirectory(porto/test/remarkinit)
#    add_subdirectory(porto/test/xyplot)
   add_subdirectory(porto/test/thirdParty/chemkinReader)
   add_subdirectory(porto/test/thirdParty/remarcReader)
   add_subdirectory(porto/test/parscale)
 endif (WITH_TESTS)

endif (WITH_PORTO)

if (WITH_TESTS)
add_subdirectory(core/test/kernel)
endif (WITH_TESTS)


# Documentation
add_subdirectory(doc)
