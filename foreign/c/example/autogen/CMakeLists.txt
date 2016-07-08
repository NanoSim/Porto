cmake_minimum_required(VERSION 3.2.1)
project (example-autogen)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
find_package(SOFT5 REQUIRED)

set (ENTITY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/entities)
set (C_SOURCE_TEMPLATE ${CMAKE_CURRENT_SOURCE_DIR}/templates/ccode.cjs)
set (C_HEADER_TEMPLATE ${CMAKE_CURRENT_SOURCE_DIR}/templates/ccode.hjs)
set (F90_MODULE_TEMPLATE ${CMAKE_CURRENT_SOURCE_DIR}/templates/fcode.fjs)

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()

add_subdirectory(entities)
add_subdirectory(fortran)
add_subdirectory(src)
