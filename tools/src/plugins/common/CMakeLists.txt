project (soft-tools-plugins-common C CXX)
set (CMAKE_THREAD_PREFER_PTHREAD TRUE)

# Expose public includes to other subprojects
set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}
  CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
