cmake_minimum_required(VERSION 3.2.1)
project (example-storageplugin C)
#set(LOCAL_ROOT_DIR "/home/sintef.no/benh/opt")
#set(SOFT5_ROOT_DIR "${LOCAL_ROOT_DIR}/lib64/soft5")
#set(_SOFT5_INCLUDEDIR "${LOCAL_ROOT_DIR}/include/soft5")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../cmake")
find_package(SOFT5 REQUIRED)

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
#  message(STATUS "Setting build type to 'Debug' as none was specified.")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()

#set (CMAKE_INCLUDE_CURRENT_DIR ON)
#set (CMAKE_THREAD_PREFER_PTHREAD TRUE)

aux_source_directory(. SRC_LIST)

set (SRC_LIST
  ${SRC_LIST}
  )
set (LINK_LIBS
  ${LINK_LIBS}
  ${SOFT5_KERNEL_LIBRARY}
  ${SOFT5_SOFTC_LIBRARY}
  )

include_directories(
  ${SOFT5_INCLUDE_DIR}
  )

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})
