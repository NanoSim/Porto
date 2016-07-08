#include <stdlib.h>
#include <string.h>

#include "softc-utils.h"

size_t
softc_strlen (const char *str)
{
  return strlen(str);
}

void
softc_strfreev (char **str)
{
  char **p;
  for (p=str; *p; p++) free(*p);
  free(str);
}
