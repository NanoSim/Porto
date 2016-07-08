#ifndef SOFTC_BYTEARRAY_H_DEF
#define SOFTC_BYTEARRAY_H_DEF

#include "softc-macros.h"
SOFTC_BEGIN_DECLS
typedef struct _softc_bytearray_s* softc_bytearray_s;

softc_bytearray_s softc_bytearray_create(const unsigned char *str, size_t len);
const unsigned char *from_softc_bytearray(const softc_bytearray_s s);
void softc_bytearray_destroy(softc_bytearray_s str);

SOFTC_END_DECLS

#endif
