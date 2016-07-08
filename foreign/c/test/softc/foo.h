#ifndef ENTITY_FOO_H_DEF
#define ENTITY_FOO_H_DEF

#include <softc/softc-datamodel.h>

typedef struct _foo_s foo_s;
typedef struct _foo_dimensions {
  size_t i_size;
} foo_dimensions;

typedef enum { YES, NO } _foo_enum_yesno;

typedef struct _foo_properties {
  int n;
  char *str;
  double * array;
} foo_properties;

#ifdef __cplusplus
extern "C" {
#endif

  foo_s *foo_create(const char *, size_t);
  void foo_free(foo_s *);
  foo_properties * const foo_props(foo_s*);
  foo_dimensions * const foo_dims(foo_s*);

  int foo_dimension_get_i (foo_s*);
  
  void foo_property_set_n (foo_s*, int);
  void foo_property_set_str (foo_s*, const char *);
  void foo_property_set_array (foo_s*, const double *, size_t);

  void foo_property_get_n (foo_s*, int*);
  void foo_property_get_str (foo_s*, char **);
  void foo_property_get_array (foo_s*, double **, size_t*);

#ifdef __cplusplus
}
#endif


#endif
