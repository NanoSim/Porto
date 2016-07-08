project (parscale-test CXX)
find_package(GTest)
if(GTEST_FOUND)

  set(CMAKE_INCLUDE_CURRENT_DIR ON)

  aux_source_directory(. SRC_LIST)
  add_executable(${PROJECT_NAME} ${SRC_LIST})
  include_directories(
    ${porto-json_INCLUDE_DIRS} 
    ${soft-storage-plugins-json_INCLUDE_DIRS}
    ${GTEST_INCLUDE_DIRS}
    )

  # TODO: Make this more elegant through using porto-json_LIBS?
  target_link_libraries(${PROJECT_NAME} 
    ${GTEST_LIBRARIES}
    )

  target_compile_features(${PROJECT_NAME} PUBLIC cxx_constexpr)
  add_test(${PROJECT_NAME}
    ${RUNNER} ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME})


  if(UNIX)
    set_property(TEST ${PROJECT_NAME} PROPERTY
      ENVIRONMENT "LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$ENV{LD_LIBRARY_PATH}")
  else()
    set_property(TEST ${PROJECT_NAME} PROPERTY
      ENVIRONMENT "PATH=${PATH}:$ENV{PATH}")
  endif()

  set_property(TEST ${PROJECT_NAME} APPEND PROPERTY
    ENVIRONMENT "SOFTSTORAGE=${SOFTSTORAGE}:$ENV{SOFTSTORAGE}")
else(GTEST_FOUND)
  message("Google test not found - skipping tests")
endif(GTEST_FOUND)
