#include "softc-string-list.h"
#include <vector>


struct _softc_string_list_s
{
  std::vector<softc_string_s> string_list;
};

softc_string_list_s* softc_string_list_create()
{
  softc_string_list_s* ret = new softc_string_list_s;
}

void softc_string_list_free(softc_string_list_s *self)
{
  while(!self->string_list.empty()) {
    softc_string_destroy(self->string_list.back());
    self->string_list.pop_back();
  }
  delete self;
}

void softc_string_list_append(softc_string_list_s *self, softc_string_s str)
{
  self->string_list.push_back(str);
}

void softc_string_list_append_cstr(softc_string_list_s *self, const char *str)
{
  softc_string_s s = softc_string_create(str);
  self->string_list.push_back(s);
}

void softc_string_list_clear(softc_string_list_s *self)
{
  while(!self->string_list.empty()) {
    softc_string_destroy(self->string_list.back());
    self->string_list.pop_back();
  }
}

size_t softc_string_list_count(const softc_string_list_s *self)
{
  return self->string_list.size();
}

softc_string_s softc_string_list_first(softc_string_list_s *self)
{
  return self->string_list.front();
}

softc_string_s softc_string_list_last(softc_string_list_s *self)
{
  return self->string_list.back();
}

//! Returns the item at index position i in the list.  i must be a
//! valid index position in the list (i.e., 0 <= i < size()).
softc_string_s softc_string_at(const softc_string_list_s *self, int index)
{
  assert(index >= 0 && index < self->string_list.size());
  return self->string_list.at(index);
}
