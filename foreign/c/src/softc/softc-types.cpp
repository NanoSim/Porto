
#include <softtypes.h>
#include "softc-types.h"

struct _softc_double_array_s
{
  soft::StdDoubleArray doubleArray;
};

softc_double_array_s *softc_double_array_create()
{
  softc_double_array_s *ret = new softc_double_array_s;
  return ret;
}

bool softc_double_array_empty (const softc_double_array_s *self)
{
  return self->doubleArray.empty();
}

size_t softc_double_array_size(const softc_double_array_s *self)
{

  return self->doubleArray.size();
}

void softc_double_array_free(softc_double_array_s **self)
{
  delete (*self);
  *self = 0;
}

size_t softc_double_array_max_size(const softc_double_array_s *self)
{
  return self->doubleArray.max_size();
}

void softc_double_array_resize(softc_double_array_s *self, size_t n)
{
  return self->doubleArray.resize(n);
}

void softc_double_array_resize_val(softc_double_array_s *self, size_t n, double val)
{
  return self->doubleArray.resize(n, val);
}

double softc_double_array_at(const softc_double_array_s *self, size_t n)
{
  return self->doubleArray.at(n);
}

void softc_double_array_foreach (softc_double_array_s *self, void (*fn)(double, void*), void *ptr)
{
  for (auto v : self->doubleArray) {
    fn(v, ptr);
  }
}
