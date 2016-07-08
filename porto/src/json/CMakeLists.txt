project (porto-json CXX)

set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC TRUE)

find_package(Qt5Core REQUIRED)

set(${PROJECT_NAME}_INCLUDE_DIRS
  ${soft-kernel_INCLUDE_DIRS}
  ${softc_INCLUDE_DIRS}
  ${porto-codegen-entities_INCLUDE_DIRS}
  ${PROJECT_SOURCE_DIR}
  CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
  
include_directories (${PROJECT_NAME} ${porto-json_INCLUDE_DIRS}  ${porto-codegen-entities_INCLUDE_DIRS} ${porto-entities_INCLUDE_DIRS})

set (LINK_LIBS Qt5::Core 
  ${softc_LIBS} 
  ${porto-codegen-entities_LIBS}
  ${porto-entities_LIBS}
  ${soft-kernel_LIBS})

aux_source_directory (. SRC_LIST)
add_library (${PROJECT_NAME} SHARED ${SRC_LIST})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_constexpr)
target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${STORAGE_PLUGINS_INSTALL_DIR})