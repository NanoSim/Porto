project (forge)

set(soft-forge_APP ${CMAKE_CURRENT_SOURCE_DIR}/soft-forge
   CACHE INTERNAL "${PROJECT_NAME}: Tool path" FORCE)

install (FILES soft-forge soft-register-entity soft-search-entity
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE  WORLD_READ WORLD_EXECUTE
  DESTINATION bin)
