project (softshell C CXX)

set (CMAKE_AUTOMOC                ON)
set (CMAKE_INCLUDE_CURRENT_DIR    ON)
set (CMAKE_THREAD_PREFER_PTHREAD  TRUE)

qt5_add_resources(QT_RESOURCE softshell.qrc)

include_directories (
  ${soft-kernel_INCLUDE_DIRS}
  ${soft-tools-plugins-common_INCLUDE_DIRS})
include_directories ("../plugins/common")
set (LINK_LIBS ${LINK_LIBS} ${HDF5_LIBRARIES} ${soft-kernel_LIBS})

aux_source_directory (. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST} ${QT_RESOURCE} softshell.qrc)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_constexpr)
target_link_libraries (${PROJECT_NAME} ${LINK_LIBS})

install (TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install (DIRECTORY modules/soft DESTINATION ${SCRIPT_MODULES_INSTALL_DIR})
install (DIRECTORY modules/utils DESTINATION ${SCRIPT_MODULES_INSTALL_DIR})
install (DIRECTORY modules/porto DESTINATION ${SCRIPT_MODULES_INSTALL_DIR})

set(softshell-modules-path ${CMAKE_CURRENT_SOURCE_DIR}/modules)

set(${PROJECT_NAME}_MODULES_DIR
  ${softshell-modules-path}
  CACHE INTERNAL "${PROJECT_NAME}: modules_dir" FORCE)

set(${PROJECT_NAME}_APPDIR
  ${softshell_BINARY_DIR}
  CACHE INTERNAL "${PROJECT_NAME}: appdir" FORCE)

set(${PROJECT_NAME}_APP
  ${softshell_BINARY_DIR}/softshell${EXEEXT}
  CACHE INTERNAL "${PROJECT_NAME}: app" FORCE)
