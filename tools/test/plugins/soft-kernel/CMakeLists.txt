project(tools-plugins-softkernel-test CXX)
find_package (GTest)
if (GTEST_FOUND)

  aux_source_directory(. SRC_LIST)
  add_executable(${PROJECT_NAME} ${SRC_LIST})
  include_directories(
    ${soft-tools-plugins-softkernel_INCLUDE_DIRS} 
    ${GTEST_INCLUDE_DIRS}
    )
  target_link_libraries(${PROJECT_NAME}
    ${soft-tools-plugins-softkernel_LIBS}
    ${GTEST_LIBRARIES})
    target_compile_features(${PROJECT_NAME} PUBLIC cxx_constexpr)

  add_test(${PROJECT_NAME}
    ${RUNNER} ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME})

  if(UNIX)
    set_property(TEST ${PROJECT_NAME} PROPERTY
      ENVIRONMENT "LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$ENV{LD_LIBRARY_PATH} ${soft-tools-plugins-softkernel_BINARY_DIR}")
  else()
    set_property(TEST ${PROJECT_NAME} PROPERTY
      ENVIRONMENT "PATH=${PATH}:$ENV{PATH}")
  endif()

else(GTEST_FOUND)
  message("Google test not found - skipping tests")
endif(GTEST_FOUND)
