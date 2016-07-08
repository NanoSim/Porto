project(storage-plugin-mongo-test2 CXX)

find_package(GTest)

if(GTEST_FOUND)

  set(CMAKE_INCLUDE_CURRENT_DIR ON)

  aux_source_directory(. SRC_LIST)
  add_executable(${PROJECT_NAME} ${SRC_LIST})
  include_directories(${soft-storage-mongo_INCLUDE_DIRS} ${GTEST_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} ${soft-storage-mongo_LIBS} ${GTEST_LIBRARIES}) 

  add_test(${PROJECT_NAME}
    ${RUNNER} ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME} "${soft-storage-plugins-mongo2_BINARY_DIR}")

  if(UNIX)
    set_property(TEST ${PROJECT_NAME} PROPERTY
      ENVIRONMENT "LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$ENV{LD_LIBRARY_PATH}:${soft-storage-plugins-mongo2_BINARY_DIR}")
  else()
    set_property(TEST ${PROJECT_NAME} PROPERTY
      ENVIRONMENT "PATH=${PATH}:$ENV{PATH}")
  endif()
 
else(GTEST_FOUND)
  message("Google test not found - skipping tests")
endif(GTEST_FOUND)
