#ifndef SOFTC_ALLOCATABLE_H_DEF
#define SOFTC_ALLOCATABLE_H_DEF
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "softc-string.h"
#include "softc-macros.h"

SOFTC_BEGIN_DECLS

typedef struct _softc_allocatable_s softc_allocatable_s;

#define DECL_ALLOCATABLE(type_t)					\
  softc_allocatable_s *softc_allocatable_create_ ## type_t (size_t rank, const size_t dims[]); \
  softc_allocatable_s *softc_allocatable_createv_ ## type_t (size_t rank, ...); \
  void softc_allocatable_free_ ## type_t(softc_allocatable_s *self);	\
  void softc_allocatable_reshape_ ## type_t(softc_allocatable_s *, size_t rank, const size_t dims[]); \
  void softc_allocatable_reshapev_ ## type_t(softc_allocatable_s *, size_t rank, ...); \
  void softc_allocatable_resize_ ## type_t(softc_allocatable_s *, size_t rank, const size_t dims[]); \
  void softc_allocatable_resizev_ ## type_t(softc_allocatable_s *, size_t rank, ...); \
  softc_allocatable_s *softc_allocatable_shallow_copy_ ## type_t (const softc_allocatable_s *); 


void softc_allocatable_dimensions(const softc_allocatable_s *, size_t *rank, size_t **dims);
void *softc_allocatable_data(softc_allocatable_s *);
void *softc_allocatable_raw_data(softc_allocatable_s *);

DECL_ALLOCATABLE(softc_string_s);
DECL_ALLOCATABLE(double);
DECL_ALLOCATABLE(float);
DECL_ALLOCATABLE(int);
DECL_ALLOCATABLE(int8_t);
DECL_ALLOCATABLE(uint8_t);
DECL_ALLOCATABLE(int16_t);
DECL_ALLOCATABLE(uint16_t);
DECL_ALLOCATABLE(int32_t);
DECL_ALLOCATABLE(uint32_t);
DECL_ALLOCATABLE(int64_t);
DECL_ALLOCATABLE(uint64_t);
DECL_ALLOCATABLE(char);

typedef void (*allocatable_free_fptr)(softc_allocatable_s*);

#define softc_allocatable_create(type_t, rank, dims)	\
  softc_allocatable_create_ ## type_t (rank, dims)

#define softc_allocatable_createv(type_t, rank,...)	\
  softc_allocatable_createv_ ## type_t (rank, ##__VA_ARGS__)

#define softc_allocatable_free(type_t, self)	\
  softc_allocatable_free_ ## type_t (self)

#define softc_allocatable_reshape(type_t, self, rank, dims)	\
  softc_allocatable_reshape_ ## type_t (self, rank, dims)

#define softc_allocatable_reshapev(type_t, self, rank, ...)	\
  softc_allocatable_reshapev_ ## type_t (self, rank, ##__VA_ARGS__)

#define softc_allocatable_resize(type_t, self, rank, dims)	\
  softc_allocatable_resize_ ## type_t (self, rank, dims)

#define softc_allocatable_resizev(type_t, self, rank, ...)	\
  softc_allocatable_resizev_ ## type_t (self, rank, ##__VA_ARGS__)

#define softc_allocatable_shallow_copy(type_t, allocatable) \
  softc_allocatable_shallow_copy_ ## type_t (allocatable) 


SOFTC_END_DECLS

#endif /* SOFTC_ALLOCATABLE_H_DEF */
