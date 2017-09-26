#include "softc-string.h"
#include "softc-string-private.hpp"
#include <string>
#include <new>

softc_string_s softc_string_create0()
{
  softc_string_s s = new _softc_string_s(nullptr);
  return s;
}

softc_string_s softc_string_create(const char *str)
{
  softc_string_s s = new _softc_string_s(str);
  return s;
}

const char *from_softc_string(const softc_string_s s)
{
  return s->str.c_str();
}

void softc_string_destroy(softc_string_s str)
{
  delete (str);
}

void softc_string_assign(softc_string_s s, const char *text)
{
  if (text) {
    s->str.assign(text);
  }
}

int softc_string_compare(softc_string_s s, const char *text)
{
  if (text) {
    return s->str.compare(text);
  } else {
    return -1;
  }
}

bool softc_string_isequal(softc_string_s first, softc_string_s second)
{
  return (first->str == second->str);
}
