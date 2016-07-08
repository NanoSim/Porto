#ifndef SOFTC_PRIVATE_H_DEF
#define SOFTC_PRIVATE_H_DEF

#ifdef __cplusplus
#  define EXTERN extern "C" 
#else
#  define EXTERN
#endif

struct _softc_t
{
  void *app;
};

EXTERN struct _softc_t *softc_private_init(int argc, char *argv[]);
EXTERN int softc_private_registered_storage_driver_count();
EXTERN char **softc_private_get_storage_drivers();
EXTERN const char *softc_private_uuidgen();
EXTERN void softc_private_cleanup(struct _softc_t *self);

#endif /* SOFTC_PRIVATE_H_DEF */
