project (dft-prepare CXX)

set (CMAKE_THREAD_PREFER_PTHREAD  TRUE)
set (CMAKE_INCLUDE_CURRENT_DIR    ON)
set (CMAKE_AUTOMOC                ON)

include_directories (
  ${soft-kernel_INCLUDE_DIRS}
  ${porto-entities_INCLUDE_DIRS}
 ) 

set (LINK_LIBS ${LINK_LIBS}
  ${soft-kernel_LIBS}
  ${porto-entities_LIBS}
  )

aux_source_directory (. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST} ${QT_RESOURCE})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_constexpr)
target_link_libraries (${PROJECT_NAME} ${LINK_LIBS})

install (TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
