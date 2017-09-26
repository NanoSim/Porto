#ifndef SOFTC_STRING_LIST_H_DEF
#define SOFTC_STRING_LIST_H_DEF

#include "softc-macros.h"
#include "softc-string.h"
SOFTC_BEGIN_DECLS

typedef struct _softc_string_list_s softc_string_list_s;

softc_string_list_s* softc_string_list_create();
void softc_string_list_free(softc_string_list_s *self);
void softc_string_list_append(softc_string_list_s *self, softc_string_s str);
void softc_string_list_append_cstr(softc_string_list_s *self, const char *str);
void softc_string_list_clear(softc_string_list_s *self);
size_t softc_string_list_count(const softc_string_list_s *self);
softc_string_s softc_string_list_first(softc_string_list_s *self);
softc_string_s softc_string_list_last(softc_string_list_s *self);
softc_string_s softc_string_at(const softc_string_list_s *self, int);

SOFTC_END_DECLS

#endif // SOFTC_STRING_LIST_H_DEF
