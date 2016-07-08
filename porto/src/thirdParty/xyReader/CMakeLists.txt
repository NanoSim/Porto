project (porto-xyreader CXX)

find_package (Qt5Core REQUIRED)

set (CMAKE_THREAD_PREFER_PTHREAD TRUE)
set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)

include_directories (${CMAKE_CURRENT_BINARY_DIR})
set (LINK_LIBS ${LINK_LIBS} Qt5::Core)
aux_source_directory(. SRC_LIST)
add_library (${PROJECT_NAME} SHARED ${SRC_LIST})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_constexpr)
target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})

# Expose public includes to other subprojects
set(${PROJECT_NAME}_INCLUDE_DIRS 
  ${PROJECT_SOURCE_DIR} 
  ${CMAKE_CURRENT_BINARY_DIR} 
  CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

set(${PROJECT_NAME}_LIBS
  ${PROJECT_NAME}
  ${LINK_LIBS}
  CACHE INTERNAL "${PROJECT_NAME}: Link Libs" FORCE)
