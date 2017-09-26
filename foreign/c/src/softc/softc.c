#include <stdlib.h>
#include "softc.h"
#include "softc-private.h"

softc_t *softc_init(int argc, char *argv[])
{
  return softc_private_init(argc, argv);
}

int softc_storage_driver_count()
{
  return softc_private_registered_storage_driver_count();
}

char ** softc_get_storage_drivers()
{
  return softc_private_get_storage_drivers();
}

const char * softc_uuidgen()
{
  return softc_private_uuidgen();
}

const char * softc_uuid_from_entity(const char *name, const char *version, const char *ns)
{
  return softc_private_uuid_from_entity(name, version, ns);
}

void softc_cleanup(softc_t* self)
{
  softc_private_cleanup(self);
}
