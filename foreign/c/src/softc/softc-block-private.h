#ifndef SOFTC_BLOCK_PRIVATE_H_DEF
#define SOFTC_BLOCK_PRIVATE_H_DEF

struct _softc_block_s
{
  size_t rank;
  size_t *dims;
  void *data;
};

#endif /* SOFTC_BLOCK_PRIVATE_H_DEF */
