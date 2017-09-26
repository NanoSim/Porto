#include <QCoreApplication>
#include <QUuid>
#include <QDebug>
#include <soft.h>
#include "softc-private.h"

struct _softc_t *softc_private_init(int argc, char *argv[])
{
  struct _softc_t *ret;
  ret = (struct _softc_t*) malloc (sizeof (struct _softc_t));
  ret->app = 0;
  soft::init(argc, argv);
  return ret;
}

int softc_private_registered_storage_driver_count()
{
  return soft::storageDriverCount();
}

char **softc_private_get_storage_drivers()
{
  char **retval = (char**)malloc(sizeof(char*) * soft::storageDriverCount() + 1);
  int i = 0;
  for (auto &driver : soft::registeredStorageDrivers()) {
    retval[i] = (char*)malloc (sizeof(char*) * driver.length());
    strcpy(retval[i], driver.data());
    retval[++i] = NULL;
  }
  return retval;
}

const char *softc_private_uuidgen()
{
  auto uuid = soft::uuidGen();
  char *ret = (char *)malloc(sizeof (char) * (uuid.length() + 1));
  memcpy(ret, uuid.data(), uuid.length());
  ret[uuid.length()] = '\0';
  return (const char*) ret;
}

const char *softc_private_uuid_from_entity(const char *name, const char *version, const char *ns)
{
  auto uuid = soft::uuidFromEntity(name, version, ns);
  char *ret = (char *)malloc(sizeof (char) * (uuid.length() + 1));
  memcpy(ret, uuid.data(), uuid.length());
  ret[uuid.length()] = '\0';
  return (const char*) ret;
}

void softc_private_cleanup(struct _softc_t *self)
{
  free (self);
  // TODO: Implement this
}
