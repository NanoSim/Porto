#ifndef FOO_H_DEF
#define FOO_H_DEF

#include <softc/softc-datamodel.h>

#ifdef _cplusplus
#define EXTERN extern "C"
#else
#define EXTERN
#endif

typedef struct _foo_s foo_s;

EXTERN foo_s *foo_create(const char *uuid);
EXTERN void foo_set_n(foo_s *, int);
EXTERN void foo_set_str(foo_s *, const char*);
		       
#endif
