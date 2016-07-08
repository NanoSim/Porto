project (soft-storage-plugins-mongo C CXX)

set (CMAKE_THREAD_PREFER_PTHREAD TRUE)
set (CMAKE_AUTOMOC TRUE)

include_directories (${soft-storage-mongo_INCLUDE_DIRS})
set (LINK_LIBS ${LINK_LIBS} ${soft-storage-mongo_LIBS})

aux_source_directory (. SRC_LIST)
add_definitions(-DQT_PLUGIN)
add_library (${PROJECT_NAME} SHARED ${SRC_LIST})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_constexpr)
target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${STORAGE_PLUGINS_INSTALL_DIR})
