#ifndef SOFTC_TRANSACTOR_H_DEF
#define SOFTC_TRANSACTOR_H_DEF

#include "softc-macros.h"

SOFT_BEGIN_DECLS

/**
 * softc_transactor_t
 *
 * The softc_transactor_t structure maintains information about the
 * transactor class
 */
typedef struct _softc_transactor_t softc_transactor_t;

softc_transactor_t    *softc_transactor_new     (const char *uri);
void                   softc_transactor_destroy (softc_transactor_t *transactor);
const char            *softc_transactor_get_uri (const softc_transactor_t *transactor);

SOFT_END_DECLS

#endif // SOFTC_TRANSACTOR_H_DEF
