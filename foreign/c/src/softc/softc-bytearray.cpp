#include "softc-bytearray.h"
#include "softc-bytearray-private.hpp"

softc_bytearray_s softc_bytearray_create(const unsigned char *data, size_t len)
{
  softc_bytearray_s s = new _softc_bytearray_s(data, len);
  return s;
}

const unsigned char *from_softc_bytearray(const softc_bytearray_s bytearray)
{
  return bytearray->data();
}

size_t softc_bytearray_size(const softc_bytearray_s bytearray)
{
  return bytearray->size();
}

void softc_bytearray_destroy(softc_bytearray_s bytearray)
{
  delete (bytearray);
}
