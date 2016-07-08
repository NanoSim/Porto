#include "softc-eventloop.h"
#include "softc-eventloop-private.h"

softc_eventloop_t* softc_eventloop_create(softc_t *softc)
{
  softc_eventloop_t *ret;
  SOFT_UNUSED(softc);
  ret = softc_eventloop_private_create();
  return ret;
}

int softc_eventloop_exec(softc_eventloop_t* eventloop)
{
  return softc_eventloop_private_exec(eventloop);
}
