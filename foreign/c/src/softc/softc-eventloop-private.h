#ifndef SOFTC_EVENTLOOP_PRIVATE_H_DEF
#define SOFTC_EVENTLOOP_PRIVATE_H_DEF

#ifdef __cplusplus
#  define EXTERNAL extern "C" 
#else
#  define EXTERNAL
#endif

struct _softc_eventloop_t
{
  void *eventLoop;
};

EXTERNAL struct _softc_eventloop_t *softc_eventloop_private_create();
EXTERNAL int softc_eventloop_private_exec(struct _softc_eventloop_t* e);
EXTERNAL void softc_eventloop_private_destroy(struct _softc_eventloop_t* e);

#endif // SOFTC_EVENTLOOP_PRIVATE_H_DEF
