#ifndef SOFTC_EVENTLOOP
#define SOFTC_EVENTLOOP

#include "softc.h"

SOFTC_BEGIN_DECLS
typedef struct _softc_eventloop_t softc_eventloop_t;

softc_eventloop_t* softc_eventloop_create(softc_t *softc);
int                softc_eventloop_exec(softc_eventloop_t* eventloop);
SOFTC_END_DECLS

#endif /* SOFTC_EVENTLOOP */
