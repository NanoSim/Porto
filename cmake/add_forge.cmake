# Run a json entity through the soft-forge code generator to create
# a header and source file
function (add_forge entity template_src output_src template_hdr output_hdr)

  if ("" STREQUAL "${softshell_APP}")
    message(FATAL_ERROR "softshell_APP not defined")
  endif()

  if ("" STREQUAL "${soft-forge_APP}")
    message(FATAL_ERROR "soft-forge_APP not defined")
  endif()

  # Note: To force the softshell to be run with the SOFT_MODULES
  #       environment variable, CMake has to be invoked as a wrapper
  #       to the softshell application, using the -E switch.
  # Note: We set some environment variables for wine to support cross-
  #       compiling on linux for Windows.  In this case RUNNER should be
  #       "wine" or "wine64".  Otherwise it will be empty.
  # Note: Dependencies are set up to softshell so that the build order
  #       is conserved even when running with a multi-processor build
  #       such as make -j. This ensures that softshell is built before
  #       being attempted run by the forge command.
  add_custom_command(
    OUTPUT ${output_hdr}
    COMMAND ${CMAKE_COMMAND} -E env "SOFT_MODULES=${softshell_MODULES_DIR}" "WINEPREFIX=$ENV{WINEPREFIX}" "WINEPATH=$ENV{WINEPATH}" ${RUNNER} ${softshell_APP} ${soft-forge_APP} ${entity} ${template_hdr} ${output_hdr}
    DEPENDS ${entity} ${C_HEADER_TEMPLATE} softshell ${soft-forge_APP}
    )

  add_custom_command(
    OUTPUT ${output_src}
    COMMAND ${CMAKE_COMMAND} -E env "SOFT_MODULES=${softshell_MODULES_DIR}" "WINEPREFIX=$ENV{WINEPREFIX}" "WINEPATH=$ENV{WINEPATH}" ${RUNNER} ${softshell_APP} ${soft-forge_APP} ${entity} ${template_src} ${output_src}
    DEPENDS ${entity} ${C_SOURCE_TEMPLATE} ${output_hdr} softshell ${soft-forge_APP}
    )

endfunction()
