#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "softc-transactor.h"
#include "softc-transactor-private.h"

softc_transactor_t *
softc_transactor_new(const char *uri_string)
{
  softc_transactor_t *transactor = NULL;
  size_t uri_length = 0;

  uri_length= strlen(uri_string) + 1;
  
  transactor = malloc (sizeof (*transactor));
  transactor->uri = (char*)malloc(uri_length * sizeof(char));
  strncpy(transactor->uri, uri_string, uri_length);

  return transactor;
}

const char *
softc_transactor_get_uri (const softc_transactor_t *transactor)
{
  return transactor->uri;
}

void
softc_transactor_destroy(softc_transactor_t *transactor)
{
  if (transactor) {
    free(transactor);
  }
}
