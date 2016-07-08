project (porto-thirdparty-remarcreader-test CXX)

find_package(GTest)

if(GTEST_FOUND)
  set(CMAKE_INCLUDE_CURRENT_DIR ON)

  aux_source_directory(. SRC_LIST)
  add_executable(${PROJECT_NAME} ${SRC_LIST})
  include_directories(${GTEST_INCLUDE_DIRS} ${porto-remarcreader_INCLUDE_DIRS})

  target_link_libraries(${PROJECT_NAME} ${GTEST_LIBRARIES} ${porto-remarcreader_LIBS})

  add_test(${PROJECT_NAME}
    ${RUNNER} ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )

  if(UNIX)
    set_property(TEST ${PROJECT_NAME} PROPERTY
      ENVIRONMENT "LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$ENV{LD_LIBRARY_PATH}")
  else()
    set_property(TEST ${PROJECT_NAME} PROPERTY
      ENVIRONMENT "PATH=${PATH}:$ENV{PATH}")
  endif()
 
else(GTEST_FOUND)
  message("Google test not found - skipping tests")
endif(GTEST_FOUND)
