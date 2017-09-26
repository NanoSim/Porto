project (soft-tools-plugins-softkernel CXX)

set (CMAKE_THREAD_PREFER_PTHREAD  TRUE)
set (CMAKE_INCLUDE_CURRENT_DIR    ON)
set (CMAKE_AUTOMOC                ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Script REQUIRED)

include_directories (
  ${soft-kernel_INCLUDE_DIRS}
  "../common"
  )

set (LINK_LIBS ${LINK_LIBS}
  ${soft-kernel_LIBS})
aux_source_directory (. SRC_LIST)
add_definitions (-DQT_PLUGIN)
add_library (${PROJECT_NAME} SHARED ${SRC_LIST})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_constexpr)
target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})

set(${PROJECT_NAME}_INCLUDE_DIRS
  ${PROJECT_SOURCE_DIR}
  ${soft-kernel_INCLUDE_DIRS}
  "${PROJECT_SOURCE_DIR}/../common"
  CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

set(${PROJECT_NAME}_LIBS
  ${PROJECT_NAME}
  ${LINK_LIBS}
  CACHE INTERNAL "${PROJECT_NAME}: Link Libs" FORCE)

#install_runtime_library(${PROJECT_NAME} ${SCRIPT_PLUGINS_INSTALL_DIR})
install(TARGETS ${PROJECT_NAME}
  LIBRARY DESTINATION ${SCRIPT_PLUGINS_INSTALL_DIR}
  ARCHIVE DESTINATION ${SCRIPT_PLUGINS_INSTALL_DIR}
  )

install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink $ENV{SOFTBASE}/${SCRIPT_PLUGINS_INSTALL_DIR}/lib${PROJECT_NAME}.so $ENV{SOFTBASE}/${LIB_INSTALL_DIR}/lib${PROJECT_NAME}.so)")
install(CODE "MESSAGE(\"-- Created symlink: $ENV{SOFTBASE}/${LIB_INSTALL_DIR}/lib${PROJECT_NAME}.so -> $ENV{SOFTBASE}/${SCRIPT_PLUGINS_INSTALL_DIR}/lib${PROJECT_NAME}.so \")")
