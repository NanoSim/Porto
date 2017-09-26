project(storage-test-mongo C CXX)

find_package(GTest)

if(GTEST_FOUND)

  set(CMAKE_INCLUDE_CURRENT_DIR ON)

  aux_source_directory(. SRC_LIST)
  add_executable(${PROJECT_NAME} ${SRC_LIST})
  include_directories(${soft-storage-mongo_INCLUDE_DIRS} ${GTEST_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} ${soft-storage-mongo_LIBS} ${GTEST_LIBRARIES})


  add_test(${PROJECT_NAME}
    ${RUNNER} ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME} "${soft-storage-plugins-mongo_BINARY_DIR}")
  add_test(${PROJECT_NAME}_build "${CMAKE_COMMAND}" --build ${CMAKE_BINARY_DIR} --target ${PROJECT_NAME})
  set_tests_properties(${PROJECT_NAME} PROPERTIES
    DEPENDS ${PROJECT_NAME}_build
    TIMEOUT 5)

  if (UNIX)
    set_property(TEST ${PROJECT_NAME}
      PROPERTY ENVIRONMENT "LD_LIBRARY_PATH=${soft-kernel_BINARY_DIR}:${soft-storage-mongo_BINARY_DIR}:${soft-storage-plugins-mongo_BINARY_DIR}:$ENV{LD_LIBRARY_PATH}")
  else()
    set_property(TEST ${PROJECT_NAME}
      PROPERTY ENVIRONMENT "PATH=${soft-kernel_BINARY_DIR}:${soft-storage-mongo_BINARY_DIR}:${soft-storage-plugins-mongo_BINARY_DIR}:$ENV{PATH}")
  endif()

else(GTEST_FOUND)
  message("Google test not found - skipping tests")
endif(GTEST_FOUND)
