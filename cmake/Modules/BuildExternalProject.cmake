#
# This function is used to force a build on a dependant project at
# cmake configuration phase.
#
# It expects to find the file "${input_dir}/build-${target}.cmake.in"
# which it will configure and evaluates the output of.
#
# This file is expected to call ExternalProject_Add() with <name> set
# to ${target} and PREFIX set to ${prefix} (but keep other directories
# to their default value).
#
# A typical value for ${prefix} would be "${CMAKE_BINARY_DIR}/usr",
# i.e. a sub-directory under the build root.
#
# Inspired by the stackoverflow answer:
# http://stackoverflow.com/questions/17446981/cmake-externalproject-add-and-findpackage/23570741#23570741
function(build_external_project target input_dir prefix)

  message(STATUS "Download, build and install ${target}")

  # Create build dir under prefix
  file(MAKE_DIRECTORY
    "${prefix}/src/${target}"
    "${prefix}/src/${target}-build"
    "${prefix}/src/${target}-trigger/build"
    )

  # Convert CMAKE_TOOLCHAIN_FILE to absolute path
  if(CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_BINARY_DIR}/${CMAKE_TOOLCHAIN_FILE}")
  endif()

  # Configure user-provided setup for ExternalProject_Add()
  configure_file(
    "${input_dir}/build-${target}.cmake.in"
    "${prefix}/src/${target}-trigger/CMakeLists.txt"
    @ONLY
    )

  # Append a custom target to the configured output
  file(APPEND "${prefix}/src/${target}-trigger/CMakeLists.txt" "

  # Add a custom target that will trigger building
  add_custom_target(trigger_${target})
  add_dependencies(trigger_${target} ${target})
")

  # Configure the external project
  execute_process(COMMAND ${CMAKE_COMMAND} ..
    WORKING_DIRECTORY "${prefix}/src/${target}-trigger/build"
    )

  # Build the external project
  execute_process(COMMAND ${CMAKE_COMMAND} --build .
    WORKING_DIRECTORY "${prefix}/src/${target}-trigger/build"
    )

endfunction()
