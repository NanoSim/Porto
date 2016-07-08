#ifndef SOFTC_TYPES_H_DEF
#define SOFTC_TYPES_H_DEF

#include "softc-macros.h"

SOFTC_BEGIN_DECLS

typedef struct _softc_double_array_s softc_double_array_s;

softc_double_array_s *softc_double_array_create();
void   softc_double_array_free       (softc_double_array_s **);
size_t softc_double_array_size       (const softc_double_array_s *);
size_t softc_double_array_max_size   (const softc_double_array_s *);
bool   softc_double_array_empty      (const softc_double_array_s *);
double softc_double_array_at         (const softc_double_array_s *self, size_t n);
void   softc_double_array_resize     (softc_double_array_s *, size_t n);
void   softc_double_array_resize_val (softc_double_array_s *, size_t n, double val);
void   softc_double_array_foreach    (softc_double_array_s *, void (*)(double, void*), void*);

SOFTC_END_DECLS

#endif /* SOFTC_TYPES_H_DEF */
