project(core-test-kernel C CXX)
set (ENTITY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/entities)
set (C_SOURCE_TEMPLATE ${CMAKE_CURRENT_SOURCE_DIR}/templates/ccode.cjs)
set (C_HEADER_TEMPLATE ${CMAKE_CURRENT_SOURCE_DIR}/templates/ccode.hjs)

add_subdirectory(entities)

find_package(GTest)

if(GTEST_FOUND)

  set(CMAKE_INCLUDE_CURRENT_DIR ON)

  aux_source_directory(. SRC_LIST)
  add_executable(${PROJECT_NAME}
    ${SRC_LIST}
    ${C_SOURCE_TEMPLATE}
    ${C_HEADER_TEMPLATE}
    ${test-entities_ENTITIES}
    )
  include_directories(
    "${SOFT_ROOT}/foreign/c/src/softc"
    ${test-entities_INCLUDE_DIRS}
    ${soft-kernel_INCLUDE_DIRS}
    ${softc_INCLUDE_DIRS}
    ${soft-storage-plugins-json_INCLUDE_DIRS}
    ${GTEST_INCLUDE_DIRS}
    )
  target_link_libraries(${PROJECT_NAME}
    ${test-entities_LIBS}
    ${soft-kernel_LIBS}
    ${softc_LIBS}
    ${soft-storage-plugins-json_LIBS}
    ${GTEST_LIBRARIES}
    )

  add_test(${PROJECT_NAME}
    ${RUNNER} ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME} "${soft-storage-plugins-mongo_BINARY_DIR}:${soft-storage-plugins-json_BINARY_DIR}")


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
