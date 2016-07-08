#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "softc-block-private.h"
#include "softc-block.h"

/*!
 * allocates a referenced memory
 */
static void *refmalloc(size_t size)
{
  void *ptr = malloc(sizeof(size_t) + size);
  size_t *cnt = (size_t *)ptr;
  *cnt = 0;
  ptr += sizeof(size_t);
  return ptr;
}


/*!
 * reallocates a referenced memory
 */
static void *refrealloc(void *data, size_t size)
{
  void *ptr = data - sizeof(size_t);
  ptr = realloc(ptr, sizeof(size_t) + size);
  size_t *cnt = (size_t *)ptr;
  *cnt = 0;
  ptr += sizeof(size_t);
  return ptr;
}


/*!
 * returns number of references
 */
size_t softc_block_data_refs(const void *ptr)
{
  size_t *cnt = (size_t*)(ptr - sizeof(size_t));
  return *cnt;
}

/*!
 * increase the reference number and return its value
 */
static size_t inc(void **mem)
{
  size_t *cnt = (size_t*)((*mem) - sizeof(size_t));
  return ++(*cnt);
}

/*!
 * decrease the reference number and return its value.
 * if all references are gone, the memory is freed.
 */
static size_t dec(void **mem)
{
  size_t *cnt = (size_t*)((*mem) - sizeof(size_t));
  if (*cnt > 0)
    return --(*cnt);
	    
  free((void*)(*mem)-sizeof(size_t));
  *mem = NULL;
  return 0;
}

softc_block_s *softc_block_shallow_copy(const softc_block_s *orig)
{
  softc_block_s *block = malloc(sizeof *block);
  size_t         size  = sizeof(*block->dims)*orig->rank;
  block->rank = orig->rank;
  block->dims = malloc(size);
  memcpy(block->dims, orig->dims, size);
  inc((void**)&orig->data);
  block->data = orig->data;
  return block;
}

softc_block_s *softc_block_create_type(size_t rank, const size_t dims[], size_t type_size)
{
  assert(rank > 0);
  softc_block_s *block = malloc(sizeof *block);
  size_t idx;
  size_t tot = 1;
  for (idx = 0; idx < rank; ++idx) {
    tot *= dims[idx];
  }
  block->rank = rank;
  block->dims = malloc(sizeof(*dims)*rank);
  memcpy(block->dims, (void*)dims, sizeof(*dims)*rank);
  block->data = refmalloc(type_size * tot);
  return block;
}

void softc_block_free(softc_block_s *block)
{
  if(block != NULL) {
    free(block->dims);
    dec((void**)&block->data);
    free(block);
  }
}

const void *softc_block_data(const softc_block_s *block)
{
  assert(block != NULL);
  return block->data;
}

const size_t *softc_block_dims(const softc_block_s *block)
{
  assert(block != NULL);
  return block->dims;
}

size_t softc_block_rank(const softc_block_s *block)
{
  assert(block != NULL);
  return block->rank;
}

void softc_block_reshape(softc_block_s *self, size_t rank, const size_t dims[])
{
  size_t tot_orig = 1;
  size_t tot_new  = 1;
  size_t idx;
  for (idx = 0; idx < self->rank; ++idx) {
    tot_orig *= self->dims[idx];
  }
  for (idx = 0; idx < rank; ++idx) {
    tot_new *= dims[idx];
  }

  /* Must reshape to the same size! */
  assert(tot_orig == tot_new);

  self->rank = rank;
  self->dims = realloc(self->dims, sizeof(*dims)*rank);
}

void softc_block_resize(softc_block_s *block, size_t rank, const size_t dims[], size_t type_size)
{
  /* warning! this command cannot be used if the block have multiple references*/ 
  assert(softc_block_data_refs(block->data) == 0);
  
  size_t idx;
  size_t tot = 1;
  for (idx = 0; idx < rank; ++idx) {
    tot *= dims[idx];
  }
  block->rank = rank;
  block->dims = realloc(block->dims, sizeof(*dims)*rank);
  memcpy(block->dims, (void*)dims, sizeof(*dims)*rank);
  block->data = refrealloc(block->data, type_size * tot);
}
