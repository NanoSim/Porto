project (softc C CXX)

set (CMAKE_THREAD_PREFER_PTHREAD TRUE)
set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)

set (LINK_LIBS ${LINK_LIBS} ${soft-kernel_LIBS})
include_directories(${PROJECT_NAME} ${soft-kernel_INCLUDE_DIRS})
aux_source_directory(. SRC_LIST)
add_library (${PROJECT_NAME} SHARED ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_constexpr)

# Expose public includes to other subprojects
set(${PROJECT_NAME}_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}/..
  CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

set(${PROJECT_NAME}_LIBS
  ${PROJECT_NAME}
  ${LINK_LIBS}
  CACHE INTERNAL "${PROJECT_NAME}: Link Libs" FORCE)

install (TARGETS ${PROJECT_NAME}
  LIBRARY DESTINATION ${LIB_INSTALL_DIR})

install(DIRECTORY .
  DESTINATION ${INCLUDE_INSTALL_DIR}/${PROJECT_NAME}
  FILES_MATCHING PATTERN "*.h"
  PATTERN "*-private.h" EXCLUDE
  )

