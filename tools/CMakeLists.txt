
option (WITH_SCRIPT_PLUGINS "Enable script plugins" OFF)
option (WITH_SOFTSHELL "Build softshell" ON)
option (WITH_HSM "Build State Machine Workflow Runner" ON)
option (WITH_WEB "Build experimental http-server" ON)

if (WITH_SCRIPT_PLUGINS)
  add_subdirectory(src/plugins/gsl)
  add_subdirectory(src/plugins/nlopt)
  add_subdirectory(src/plugins/hdf5)
  add_subdirectory(src/plugins/hsm)
endif (WITH_SCRIPT_PLUGINS)

if (WITH_SOFTSHELL)
  add_subdirectory(src/softshell)
endif (WITH_SOFTSHELL)

if (WITH_WEB)
  add_subdirectory(src/web)
endif (WITH_WEB)
