project (soft-tools-hsm C CXX)

set (CMAKE_THREAD_PREFER_PTHREAD  TRUE)
set (CMAKE_INCLUDE_CURRENT_DIR    ON)
set (CMAKE_AUTOMOC                ON)

include_directories (
  "../common")

# Export/import declaration for Windows

aux_source_directory (. SRC_LIST)
set (LINK_LIBS ${LINK_LIBS}
  Qt5::Core
  Qt5::Script
  Qt5::Concurrent)

add_definitions(-DQT_PLUGIN)
add_library (${PROJECT_NAME} SHARED ${SRC_LIST})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_constexpr)
target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})
install_runtime_library(${PROJECT_NAME} ${SCRIPT_PLUGINS_INSTALL_DIR})
install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink $ENV{SOFTBASE}/${SCRIPT_PLUGINS_INSTALL_DIR}/lib${PROJECT_NAME}.so $ENV{SOFTBASE}/${LIB_INSTALL_DIR}/lib${PROJECT_NAME}.so)")
install(CODE "MESSAGE(\"-- Created symlink: $ENV{SOFTBASE}/${LIB_INSTALL_DIR}/lib${PROJECT_NAME}.so -> $ENV{SOFTBASE}/${SCRIPT_PLUGINS_INSTALL_DIR}/lib${PROJECT_NAME}.so \")")
