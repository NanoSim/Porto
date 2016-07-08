#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <assert.h>

#include "softc-block.h"
#include "softc-block-private.h"
#include "softc-allocatable.h"
#include "softc-string.h"

struct _softc_allocatable_s
{
  softc_block_s *block;
  void          *data;
};

#define MAX_RANK 4

#define IMPL_ALLOC_RANK1(type_t) \
  static softc_allocatable_s* alloc_rank1_ ## type_t(softc_allocatable_s* alloc) \
  {									\
    alloc->data = (void*)softc_block_data(alloc->block);		\
    return alloc;							\
  }

#define IMPL_ALLOC_RANK2(type_t)					\
  static softc_allocatable_s* alloc_rank2_ ## type_t(softc_allocatable_s* alloc) \
  {									\
    const size_t *dims = softc_block_dims(alloc->block);		\
    const size_t nj = dims[1];						\
    const size_t ni = dims[0];						\
    size_t j;								\
    type_t **vec = malloc(sizeof(*vec) * nj);				\
    for (j = 0; j < nj; ++j) {						\
      vec[j] = (type_t*)alloc->block->data + (j*ni);			\
    }									\
    alloc->data = (void*)vec;						\
    return alloc;							\
  }

#define IMPL_ALLOC_RANK3(type_t)					\
  static softc_allocatable_s* alloc_rank3_ ## type_t (softc_allocatable_s* alloc) \
  {									\
    const size_t nk = alloc->block->dims[2];				\
    const size_t nj = alloc->block->dims[1];				\
    const size_t ni = alloc->block->dims[0];				\
    size_t j, k;							\
    type_t ***vec = malloc(sizeof(*vec) * nk);				\
    for (k = 0; k < nk; ++k) {						\
      vec[k] = malloc(sizeof(**vec) * nj);				\
      for (j = 0; j < nj; ++j) {					\
	vec[k][j] = (type_t*)alloc->block->data + (j*ni) + (k*ni*nj);	\
      }									\
    }									\
    alloc->data = (void*)vec;						\
    return alloc;							\
}

#define IMPL_ALLOC_RANK4(type_t)					\
  static softc_allocatable_s* alloc_rank4_ ## type_t (softc_allocatable_s* alloc) \
  {									\
    const size_t ni = alloc->block->dims[0];				\
    const size_t nj = alloc->block->dims[1];				\
    const size_t nk = alloc->block->dims[2];				\
    const size_t nl = alloc->block->dims[3];				\
    									\
    size_t j, k, l;							\
    type_t ****vec = malloc(sizeof(*vec) * nl);				\
    for (l = 0; l < nl; ++l) {						\
      vec[l] = malloc(sizeof(***vec) * nk);				\
      for (k = 0; k < nk; ++k) {					\
	vec[l][k] = malloc(sizeof(**vec) * nj);				\
	for (j = 0; j < nj; ++j) {					\
	  vec[l][k][j] = (type_t*)alloc->block->data + (j*ni) + (k*ni*nj) + (l*ni*nj*nk); \
	}								\
      }									\
    }									\
    alloc->data = (void*)vec;						\
    return alloc;							\
  }

static void free_rank1(softc_allocatable_s* alloc)	
{							
  (void)(alloc);					
}

#define IMPL_FREE_RANK2(type_t)					\
  static void free_rank2_ ## type_t(softc_allocatable_s* alloc)	\
  {								\
    type_t **vec = alloc->data;					\
    free(vec);							\
  }

#define IMPL_FREE_RANK3(type_t)						\
  static void free_rank3_ ## type_t (softc_allocatable_s* alloc)	\
  {									\
    const size_t nk = alloc->block->dims[2];				\
    size_t k;								\
    type_t ***vec = alloc->data;					\
    for (k = 0; k < nk; ++k) {						\
      free(vec[k]);							\
    }									\
    free(vec);								\
  }

#define IMPL_FREE_RANK4(type_t)						\
  static void free_rank4_ ## type_t (softc_allocatable_s* alloc)	\
  {									\
    const size_t nk = alloc->block->dims[2];				\
    const size_t nl = alloc->block->dims[3];				\
    size_t k, l;							\
    type_t ***vec = alloc->data;					\
    for (l = 0; l < nl; ++l) {						\
      for (k = 0; k < nk; ++k) {					\
	free(vec[l][k]);						\
      }									\
      free(vec[l]);							\
    }									\
    free(vec);								\
  }

typedef softc_allocatable_s*(*alloc_fptr)(softc_allocatable_s*);
typedef void(*free_fptr)(softc_allocatable_s*);

#define ALLOC_FPTRS(type_t)				\
  static alloc_fptr alloc_fptrs_ ## type_t [] = {	\
    alloc_rank1_ ## type_t,				\
    alloc_rank2_ ## type_t,				\
    alloc_rank3_ ## type_t,				\
    alloc_rank4_ ## type_t				\
  };

#define FREE_FPTRS(type_t)						\
  static free_fptr free_fptrs_ ## type_t [] = {				\
    free_rank1,								\
    free_rank2_ ## type_t,						\
    free_rank3_ ## type_t,						\
    free_rank4_ ## type_t						\
  };


#define IMPL_ALLOCATABLE_CREATE(type_t)					\
  softc_allocatable_s *softc_allocatable_create_ ## type_t (size_t rank, const size_t dims[]) \
  {									\
    softc_allocatable_s *allocatable = malloc(sizeof *allocatable);	\
    allocatable->block = softc_block_create_type(rank, dims, sizeof(type_t*)); \
    return alloc_fptrs_ ## type_t [rank-1](allocatable);		\
  }

#define IMPL_ALLOCATABLE_CREATEV(type_t)				\
  softc_allocatable_s *softc_allocatable_createv_ ## type_t (const size_t rank, ...) \
  {									\
    assert(rank > 0 && rank <= MAX_RANK);				\
    size_t i;								\
    va_list vl;								\
    size_t dims[MAX_RANK];						\
    va_start(vl, rank);							\
    for (i = 0; i < rank; i++) {					\
      dims[i] = va_arg(vl, const size_t);				\
    }									\
    va_end(vl);								\
    return softc_allocatable_create_ ## type_t(rank, dims);		\
  }

#define IMPL_SOFTC_ALLOCATABLE_FREE(type_t)				\
  void softc_allocatable_free_ ## type_t (softc_allocatable_s *self)	\
  {									\
    free_fptrs_ ## type_t [self->block->rank-1](self);			\
    softc_block_free(self->block);					\
    free(self);								\
  }									\


#define IMPL_SOFTC_ALLOCATABLE_RESHAPE(type_t)				\
  void softc_allocatable_reshape_ ## type_t (softc_allocatable_s *self, size_t rank, const size_t dims[]) \
  {									\
    assert(rank > 0 && rank <= MAX_RANK);				\
    free_fptrs_ ## type_t[self->block->rank-1](self);			\
    softc_block_reshape (self->block, rank, dims);			\
    alloc_fptrs_ ## type_t[self->block->rank-1](self);			\
  }									\
  
#define IMPL_SOFTC_ALLOCATABLE_RESHAPEV(type_t)				\
  void softc_allocatable_reshapev_ ## type_t (softc_allocatable_s *self, size_t rank, ...) \
  {									\
    assert(rank > 0 && rank <= MAX_RANK);				\
    size_t i;								\
    va_list vl;								\
    size_t dims[MAX_RANK];						\
    va_start(vl, rank);							\
    for (i = 0; i < rank; i++) {					\
      dims[i] = va_arg(vl, const size_t);				\
    }									\
    va_end(vl);								\
    free_fptrs_ ## type_t[self->block->rank-1](self);			\
    softc_block_reshape(self->block, rank, dims);			\
    alloc_fptrs_ ## type_t[self->block->rank-1](self);			\
  }

#define IMPL_SOFTC_ALLOCATABLE_RESIZE(type_t)				\
  void softc_allocatable_resize_ ## type_t (softc_allocatable_s *self, size_t rank, const size_t dims[]) \
  {									\
    assert(rank > 0 && rank <= MAX_RANK);				\
    free_fptrs_ ## type_t[self->block->rank-1](self);			\
    softc_block_resize (self->block,rank,dims,sizeof(type_t*));		\
    alloc_fptrs_ ## type_t[self->block->rank-1](self);			\
  }									\
  
#define IMPL_SOFTC_ALLOCATABLE_RESIZEV(type_t)				\
  void softc_allocatable_resizev_ ## type_t (softc_allocatable_s *self, size_t rank, ...) \
  {									\
    assert(rank > 0 && rank <= MAX_RANK);				\
    size_t i;								\
    va_list vl;								\
    size_t dims[MAX_RANK];						\
    va_start(vl, rank);							\
    for (i = 0; i < rank; i++) {					\
      dims[i] = va_arg(vl, const size_t);				\
    }									\
    va_end(vl);								\
    free_fptrs_ ## type_t[self->block->rank-1](self);			\
    softc_block_resize(self->block,rank,dims,sizeof(type_t*));		\
    alloc_fptrs_ ## type_t[self->block->rank-1](self);			\
  }

#define IMPL_SOFTC_ALLOCATABLE_SHALLOW_COPY(type_t)			\
  softc_allocatable_s *softc_allocatable_shallow_copy_ ## type_t(const softc_allocatable_s *orig) \
  {									\
    softc_allocatable_s *allocatable = malloc(sizeof *allocatable);	\
    size_t rank = orig->block->rank;					\
    allocatable->block = softc_block_shallow_copy(orig->block);		\
    return alloc_fptrs_ ## type_t[rank-1](allocatable);			\
  }


#define IMPL_ALLOC(type_t)				\
  IMPL_ALLOC_RANK1(type_t)				\
  IMPL_ALLOC_RANK2(type_t)				\
  IMPL_ALLOC_RANK3(type_t)				\
  IMPL_ALLOC_RANK4(type_t)				\
  IMPL_FREE_RANK2(type_t)				\
  IMPL_FREE_RANK3(type_t)				\
  IMPL_FREE_RANK4(type_t)				\
  ALLOC_FPTRS(type_t)					\
  FREE_FPTRS(type_t)					\
  IMPL_ALLOCATABLE_CREATE(type_t)			\
  IMPL_ALLOCATABLE_CREATEV(type_t)			\
  IMPL_SOFTC_ALLOCATABLE_FREE(type_t)			\
  IMPL_SOFTC_ALLOCATABLE_RESHAPE(type_t)		\
  IMPL_SOFTC_ALLOCATABLE_RESHAPEV(type_t)		\
  IMPL_SOFTC_ALLOCATABLE_RESIZE(type_t)			\
  IMPL_SOFTC_ALLOCATABLE_RESIZEV(type_t)		\
  IMPL_SOFTC_ALLOCATABLE_SHALLOW_COPY(type_t)

IMPL_ALLOC(char);
IMPL_ALLOC(softc_string_s);
IMPL_ALLOC(double);
IMPL_ALLOC(float);
IMPL_ALLOC(int);
IMPL_ALLOC(int8_t);
IMPL_ALLOC(uint8_t);
IMPL_ALLOC(int16_t);
IMPL_ALLOC(uint16_t);
IMPL_ALLOC(int32_t);
IMPL_ALLOC(uint32_t);
IMPL_ALLOC(int64_t);
IMPL_ALLOC(uint64_t);

void softc_allocatable_dimensions(const softc_allocatable_s *self, size_t *rank, size_t **dims)
{
  *rank = self->block->rank;
  *dims = self->block->dims;  
}

/*!
 * Returns pointer to the reference
 */
void *softc_allocatable_data(softc_allocatable_s *self)
{
  return self->data;
}

/*!
 * Returns pointer to the continous raw data for the allocated block
 */
void *softc_allocatable_raw_data(softc_allocatable_s *self)
{
  return self->block->data;
}
