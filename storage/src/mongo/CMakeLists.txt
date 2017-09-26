project (soft-storage-mongo C CXX)

set (CMAKE_THREAD_PREFER_PTHREAD  TRUE)
set (CMAKE_INCLUDE_CURRENT_DIR    ON)
set (CMAKE_AUTOMOC                TRUE)

include_directories (${MONGOC_INCLUDE_DIR} ${soft-kernel_INCLUDE_DIRS})
set (LINK_LIBS ${LINK_LIBS} ${BSON_LIBRARY} ${MONGOC_LIBRARY} ${soft-kernel_LIBS})

aux_source_directory (. SRC_LIST)
add_library (${PROJECT_NAME} SHARED ${SRC_LIST})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_constexpr)
target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})

MESSAGE(STATUS "soft-storage-mongo libs: ${LINK_LIBS}")
# Expose public includes to other subprojects
set(${PROJECT_NAME}_INCLUDE_DIRS
  ${PROJECT_SOURCE_DIR}
  ${MONGOC_INCLUDE_DIR}
  ${soft-kernel_INCLUDE_DIRS}
  CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

set(${PROJECT_NAME}_LIBS
  ${PROJECT_NAME}
  ${LINK_LIBS}
  CACHE INTERNAL "${PROJECT_NAME}: Link Libs" FORCE)

install(TARGETS ${PROJECT_NAME}
  LIBRARY DESTINATION ${LIB_INSTALL_DIR}
  ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
)
install(DIRECTORY .
  DESTINATION ${INCLUDE_INSTALL_DIR}
  FILES_MATCHING PATTERN "*.h"
  )
install(FILES QMongo DESTINATION ${INCLUDE_INSTALL_DIR})
