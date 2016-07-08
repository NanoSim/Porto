project (soft-tools-plugins-nlopt C CXX)

set (CMAKE_THREAD_PREFER_PTHREAD  TRUE)
set (CMAKE_INCLUDE_CURRENT_DIR    ON)
set (CMAKE_AUTOMOC                ON)

find_package(NLOPT REQUIRED)

include_directories (
  ${NLOPT_INCLUDE_DIR}
  ${soft-kernel_INCLUDE_DIRS}
  "../common")

set (LINK_LIBS ${LINK_LIBS} ${NLOPT_LIBRARIES} ${soft-kernel_LIBS})
aux_source_directory (. SRC_LIST)

add_definitions(-DQT_PLUGIN)
add_library (${PROJECT_NAME} SHARED ${SRC_LIST})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_constexpr)
target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${SCRIPT_PLUGINS_INSTALL_DIR})
