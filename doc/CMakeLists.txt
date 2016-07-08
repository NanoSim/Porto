project(soft5-documentation)
find_package(Doxygen)

if (DOXYGEN_FOUND)
  set(dependencies "")
  
  if (WITH_FOREIGN_PYTHON)
    set(dependencies ${dependencies};${CMAKE_CURRENT_BINARY_DIR}/softpy.py)
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/softpy.py
      COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/formatSwigPythonModule.py ${softpy_BINARY_DIR}/softpy.py ${CMAKE_CURRENT_BINARY_DIR}/softpy.py
      DEPENDS ${softpy_BINARY_DIR}/softpy.py
      )
  endif()
  
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
  add_custom_target(doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen" VERBATIM
    DEPENDS ${dependencies}
    )  
endif (DOXYGEN_FOUND)
