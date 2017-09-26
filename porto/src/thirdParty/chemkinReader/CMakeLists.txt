project (porto-chemkinreader C CXX)

find_package(Boost 1.58.0 REQUIRED COMPONENTS regex)

set (CMAKE_THREAD_PREFER_PTHREAD TRUE)
set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)

include_directories (${CMAKE_CURRENT_BINARY_DIR} ${Boost_INCLUDE_DIR})
set (LINK_LIBS ${LINK_LIBS}
    ${Boost_REGEX_LIBRARY_RELEASE}
)

aux_source_directory(. SRC_LIST)
add_library (${PROJECT_NAME} SHARED ${SRC_LIST})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_constexpr)
target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})

# Expose public includes to other subprojects
set(${PROJECT_NAME}_INCLUDE_DIRS 
  ${PROJECT_SOURCE_DIR} 
  ${CMAKE_CURRENT_BINARY_DIR} 
  ${Boost_INCLUDE_DIR}
  CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

set(${PROJECT_NAME}_LIBS
  ${PROJECT_NAME}
  ${LINK_LIBS}
  CACHE INTERNAL "${PROJECT_NAME}: Link Libs" FORCE)

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${LIB_INSTALL_DIR})
