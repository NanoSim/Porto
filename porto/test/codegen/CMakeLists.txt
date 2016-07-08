project(porto-codegen-test CXX)
set (ENTITY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/entities)
set (CPP_SOURCE_TEMPLATE ${SOFT_ROOT}/porto/src/templates/cppcode.cjs)
set (CPP_HEADER_TEMPLATE ${SOFT_ROOT}/porto/src/templates/cppcode.hjs)

add_subdirectory(entities)
find_package(GTest)

if(GTEST_FOUND)

  set(CMAKE_INCLUDE_CURRENT_DIR ON)
  add_definitions(-DSOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
  aux_source_directory(. SRC_LIST)
  add_executable(${PROJECT_NAME} ${SRC_LIST} ${CPP_SOURCE_TEMPLATE} ${CPP_HEADER_TEMPLATE})
  include_directories(${GTEST_INCLUDE_DIRS} ${soft-kernel_INCLUDE_DIRS} ${porto-codegen-entities_INCLUDE_DIRS} ${porto-entities_INCLUDE_DIRS} ${porto-chemkinreader_INCLUDE_DIRS})
  target_compile_features(${PROJECT_NAME} PUBLIC cxx_constexpr)
  target_link_libraries(${PROJECT_NAME} ${GTEST_LIBRARIES} ${soft-kernel_LIBS} ${porto-codegen-entities_LIBS} ${porto-entities_LIBS} ${porto-chemkinreader_LIBS})

  add_test(${PROJECT_NAME}
    ${RUNNER} ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME} "${soft-storage-plugins-mongo2_BINARY_DIR}")

  if(UNIX)
    set_property(TEST ${PROJECT_NAME} PROPERTY
      ENVIRONMENT "LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$ENV{LD_LIBRARY_PATH}:${porto-codegen-entities_BINARY_DIR}:${porto-chemkinreader_BINARY_DIR}:${soft-storage-plugins-mongo2_BINARY_DIR}")
  else()
    set_property(TEST ${PROJECT_NAME} PROPERTY
      ENVIRONMENT "PATH=${PATH}:$ENV{PATH}")
  endif()

else(GTEST_FOUND)
  message("Google test not found - skipping tests")
endif(GTEST_FOUND)
