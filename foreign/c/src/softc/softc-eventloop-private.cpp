#include <stdlib.h>
#include <QEventLoop>
#include "softc-eventloop-private.h"

struct _softc_eventloop_t *softc_eventloop_private_create()
{
  struct _softc_eventloop_t *e;
  e = (struct _softc_eventloop_t *)malloc(sizeof (struct _softc_eventloop_t));
  e->eventLoop = (void*)new QEventLoop();
  return e;
}

int softc_eventloop_private_exec(struct _softc_eventloop_t* e)
{
  QEventLoop *eventLoop = (QEventLoop*)e->eventLoop;
  return eventLoop->exec(QEventLoop::AllEvents);
}

void softc_eventloop_private_destroy(struct _softc_eventloop_t* e)
{
  //QEventLoop *eventLoop = (QEventLoop*)e->eventLoop;
  
}
