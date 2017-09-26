# Installation declaration that can account for the difference in behavior between
# Linux and Windows regarding shared libraries. In Linux CMake considers them libraries,
# while on Windows CMake considers them runtime.
function (install_runtime_library libname destination)
   if (WIN32)
      install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${LIB_INSTALL_DIR})
   else()
      install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${LIB_INSTALL_DIR})
   endif()
endfunction()
