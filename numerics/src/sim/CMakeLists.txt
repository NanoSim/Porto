project (soft-numerics-sim C CXX)

set (CMAKE_INCLUDE_CURRENT_DIR    ON)
set (CMAKE_THREAD_PREFER_PTHREAD  TRUE)
set (CMAKE_AUTOMOC                TRUE)
set (LINK_LIBS ${LINK_LIBS}
  ${MPI_LIBRARIES}
  ${softc_LIBS})



include_directories (${softc_INCLUDE_DIRS})
configure_file ("config.h.in" "config.h")

aux_source_directory (. SRC_LIST)
add_library(${PROJECT_NAME} SHARED ${SRC_LIST} )

target_compile_features(${PROJECT_NAME} PUBLIC cxx_constexpr)
target_link_libraries (${PROJECT_NAME} ${LINK_LIBS})

set(${PROJECT_NAME}_INCLUDE_DIRS
  ${PROJECT_SOURCE_DIR}
  CACHE INTERNAL "${PROJECT_NAME}: HEADERS" FORCE)

set(${PROJECT_NAME}_LIBS
  ${PROJECT_NAME}
  ${LINK_LIBS}
  CACHE INTERNAL "${PROJECT_NAME}: LIBS" FORCE)

install (TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${LIB_INSTALL_DIR})
install (DIRECTORY .
  DESTINATION ${INCLUDE_INSTALL_DIR}/sim
  FILES_MATCHING PATTERN "*.h"
  PATTERN "*-private.h" EXCLUDE
  )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/config.h DESTINATION ${INCLUDE_INSTALL_DIR})

