project (porto-xyplot C CXX)

set (CMAKE_THREAD_PREFER_PTHREAD  TRUE)
set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC TRUE)

include_directories (${PROJECT_NAME}
  ${porto-entities_INCLUDE_DIRS}
  )

set (LINK_LIBS ${LINK_LIBS}
  ${porto-entities_LIBS}
  )

aux_source_directory (. SRC_LIST)
add_library (${PROJECT_NAME} SHARED ${SRC_LIST})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_constexpr)
target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})

set(${PROJECT_NAME}_INCLUDE_DIRS
  ${PROJECT_SOURCE_DIR}
  ${porto-entities_INCLUDE_DIRS}
  CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

set(${PROJECT_NAME}_LIBS
  ${PROJECT_NAME}
  ${LINK_LIBS}
  CACHE INTERNAL "${PROJECT_NAME}: Link Libs" FORCE)

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${STORAGE_PLUGINS_INSTALL_DIR})