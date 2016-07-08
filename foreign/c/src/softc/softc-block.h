#ifndef SOFTC_BLOCK_H_DEF
#define SOFTC_BLOCK_H_DEF

typedef struct _softc_block_s softc_block_s;

softc_block_s *softc_block_create_type(size_t rank, const size_t dims[], size_t type_size);
void           softc_block_free(softc_block_s *block);
const void    *softc_block_data(const softc_block_s *block);
const size_t  *softc_block_dims(const softc_block_s *block);
size_t         softc_block_rank(const softc_block_s *block);
size_t         softc_block_data_refs(const void *ptr);
void           softc_block_reshape(softc_block_s *block, size_t rank, const size_t dims[]);
void           softc_block_resize(softc_block_s *block, size_t rank, const size_t dims[], size_t type_type);
softc_block_s *softc_block_shallow_copy(const softc_block_s *orig);
#endif /* SOFTC_BLOCK_H_DEF */
