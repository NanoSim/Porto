cmake_minimum_required(VERSION 3.2)
project (entities C CXX)

set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_THREAD_PREFER_PTHREAD TRUE)
set (SRC_LIST ${SRC_LIST})

# Run a json entity through the soft-forge code generator to create
# a header and source file
function (add_forge entity output_src output_hdr output_f90)
  add_custom_command(
    OUTPUT ${output_f90}
    COMMAND soft-forge ${entity} ${F90_MODULE_TEMPLATE} ${output_f90}
    DEPENDS ${entity} ${F90_MODULE_TEMPLATE} 
    )

  add_custom_command(
    OUTPUT ${output_hdr}
    COMMAND soft-forge ${entity} ${C_HEADER_TEMPLATE} ${output_hdr}
    DEPENDS ${entity} ${C_HEADER_TEMPLATE} ${output_f90}
    )

  add_custom_command(
    OUTPUT ${output_src}
    COMMAND soft-forge ${entity} ${C_SOURCE_TEMPLATE} ${output_src}
    DEPENDS ${entity} ${C_SOURCE_TEMPLATE} ${output_hdr}
    )

endfunction()


# Find all .json files in current source dir and run through
# soft-forge
FILE(GLOB json-sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.json)
foreach(JSON_SRC ${json-sources})
  string(REGEX REPLACE "\.json$" ".c" GEN_SRC ${JSON_SRC})
  string(REGEX REPLACE "\.json$" ".h" GEN_HDR ${JSON_SRC})
  string(REGEX REPLACE "\.json$" ".f90" GEN_F90 ${JSON_SRC})
  add_forge(${CMAKE_CURRENT_SOURCE_DIR}/${JSON_SRC}
    ${example-autogen_BINARY_DIR}/entities/${GEN_SRC}
    ${example-autogen_BINARY_DIR}/entities/${GEN_HDR}
    ${example-autogen_BINARY_DIR}/entities/${GEN_F90})
  list(APPEND SRC_LIST ${GEN_SRC})  
endforeach(JSON_SRC ${json-sources})

include_directories(${SOFT5_INCLUDE_DIR})
set (LINK_LIBS ${LINK_LIBS}
  ${SOFT5_KERNEL_LIBRARY}
  ${SOFT5_SOFTC_LIBRARY})

add_library(${PROJECT_NAME} SHARED ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})

# Expose public includes to other subprojects
set(${PROJECT_NAME}_INCLUDE_DIRS
  ${CMAKE_CURRENT_BINARY_DIR}
  ${SOFT5_INCLUDE_DIR}
  CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

set(${PROJECT_NAME}_LIBS
  ${PROJECT_NAME}
  ${LINK_LIBS}
  CACHE INTERNAL "${PROJECT_NAME}: Link Libs" FORCE)
