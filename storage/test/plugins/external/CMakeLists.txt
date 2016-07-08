project(storage-plugin-external-test C CXX)
set (ENTITY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/entities)
set (C_SOURCE_TEMPLATE ${CMAKE_CURRENT_SOURCE_DIR}/templates/ccode.cjs)
set (C_HEADER_TEMPLATE ${CMAKE_CURRENT_SOURCE_DIR}/templates/ccode.hjs)

add_subdirectory(entities)
find_package(GTest)

if(GTEST_FOUND)

  set(CMAKE_INCLUDE_CURRENT_DIR ON)

  aux_source_directory(. SRC_LIST)
  add_executable(${PROJECT_NAME} ${SRC_LIST})
  include_directories(${plot-entities_INCLUDE_DIRS} ${soft-storage-plugins-external_INCLUDE_DIRS} ${GTEST_INCLUDE_DIRS})
  target_compile_features(${PROJECT_NAME} PUBLIC cxx_constexpr)
  target_link_libraries(${PROJECT_NAME} ${plot-entities_LIBS} ${GTEST_LIBRARIES}) 

  add_test(${PROJECT_NAME}
    ${RUNNER} ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME} "${soft-storage-plugins-external_BINARY_DIR}")

  if(UNIX)
    set_property(TEST ${PROJECT_NAME} PROPERTY
      ENVIRONMENT "LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$ENV{LD_LIBRARY_PATH}:${plot-entities_BINARY_DIR}:${soft-storage-plugins-external_BINARY_DIR}:${porto-json_BINARY_DIR}")
  else()
    set_property(TEST ${PROJECT_NAME} PROPERTY
      ENVIRONMENT "PATH=${PATH}:$ENV{PATH}")
  endif()

  set_property(TEST ${PROJECT_NAME} APPEND PROPERTY
    ENVIRONMENT "SOFTSTORAGE=${SOFTSTORAGE}:$ENV{SOFTSTORAGE}:${soft-storage-plugins-external_BINARY_DIR}:${porto-json_BINARY_DIR}")
 
else(GTEST_FOUND)
  message("Google test not found - skipping tests")
endif(GTEST_FOUND)
