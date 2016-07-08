project (porto-entities C CXX)

set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_THREAD_PREFER_PTHREAD TRUE)
set (SRC_LIST ${SRC_LIST})
set (ENTITY_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set (C_SOURCE_TEMPLATE ${CMAKE_CURRENT_SOURCE_DIR}/../templates/ccode.cjs)
set (C_HEADER_TEMPLATE ${CMAKE_CURRENT_SOURCE_DIR}/../templates/ccode.hjs)
set (CPP_SOURCE_TEMPLATE ${CMAKE_CURRENT_SOURCE_DIR}/../templates/cppcode.cjs)
set (CPP_HEADER_TEMPLATE ${CMAKE_CURRENT_SOURCE_DIR}/../templates/cppcode.hjs)

# Find all .json files in current source dir and run through
# soft-forge
FILE(GLOB json-sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.json)
foreach(JSON_SRC ${json-sources})
  string(REGEX REPLACE "\.json$" ".c" GEN_SRC ${JSON_SRC})
  string(REGEX REPLACE "\.json$" ".h" GEN_HDR ${JSON_SRC})
  string(REGEX REPLACE "\.json$" ".cxx" GEN_CPPSRC ${JSON_SRC})
  string(REGEX REPLACE "\.json$" ".hxx" GEN_CPPHDR ${JSON_SRC})
  add_forge(${CMAKE_CURRENT_SOURCE_DIR}/${JSON_SRC}
    ${C_SOURCE_TEMPLATE} ${CMAKE_CURRENT_BINARY_DIR}/${GEN_SRC}
    ${C_HEADER_TEMPLATE} ${CMAKE_CURRENT_BINARY_DIR}/${GEN_HDR})

  add_forge(${CMAKE_CURRENT_SOURCE_DIR}/${JSON_SRC}
    ${CPP_SOURCE_TEMPLATE} ${CMAKE_CURRENT_BINARY_DIR}/${GEN_CPPSRC}
    ${CPP_HEADER_TEMPLATE} ${CMAKE_CURRENT_BINARY_DIR}/${GEN_CPPHDR})

  list(APPEND SRC_LIST ${GEN_SRC})
  list(APPEND SRC_LIST ${GEN_CPPSRC})
endforeach(JSON_SRC ${json-sources})

set_source_files_properties(${SRC_LIST} PROPERTIES GENERATED TRUE)
include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${soft-kernel_INCLUDE_DIRS}
  ${softc_INCLUDE_DIRS})

set (LINK_LIBS ${LINK_LIBS}
  ${soft-kernel_LIBS}
  ${softc_LIBS})

add_library(${PROJECT_NAME} STATIC ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})

# Expose public includes to other subprojects
set(${PROJECT_NAME}_INCLUDE_DIRS
  ${CMAKE_CURRENT_BINARY_DIR}
  ${softc_INCLUDE_DIRS}
  ${soft-kernel_INCLUDE_DIRS}
  CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

set(${PROJECT_NAME}_LIBS
  ${PROJECT_NAME}
  ${LINK_LIBS}
  CACHE INTERNAL "${PROJECT_NAME}: Link Libs" FORCE)
