project (soft-tools-plugins-mongo C CXX)

set (CMAKE_THREAD_PREFER_PTHREAD  TRUE)
set (CMAKE_INCLUDE_CURRENT_DIR    ON)
set (CMAKE_AUTOMOC                ON)

find_package(Qt5Core REQUIRED)

include_directories (
  ${soft-storage-mongo_INCLUDE_DIRS}
  "../common")

# Export/import declaration for Windows
if (WIN32)
generate_export_header(${PROJECT_NAME})
endif()

set (LINK_LIBS ${LINK_LIBS} ${soft-storage-mongo_LIBS})
aux_source_directory (. SRC_LIST)

add_definitions(-DQT_PLUGIN)
add_library (${PROJECT_NAME} SHARED ${SRC_LIST})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_constexpr)
target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})

# Export/import declaration for Windows
if (WIN32)
generate_export_header(${PROJECT_NAME})
endif()

if (WIN32)
install_runtime_library(${PROJECT_NAME} ${SCRIPT_PLUGINS_INSTALL_DIR})
else ()
install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${SCRIPT_PLUGINS_INSTALL_DIR})
endif()
install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink $ENV{SOFTBASE}/${SCRIPT_PLUGINS_INSTALL_DIR}/lib${PROJECT_NAME}.so $ENV{SOFTBASE}/${LIB_INSTALL_DIR}/lib${PROJECT_NAME}.so)")
install(CODE "MESSAGE(\"-- Created symlink: $ENV{SOFTBASE}/${LIB_INSTALL_DIR}/lib${PROJECT_NAME}.so -> $ENV{SOFTBASE}/${SCRIPT_PLUGINS_INSTALL_DIR}/lib${PROJECT_NAME}.so \")")
