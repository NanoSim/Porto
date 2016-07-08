#ifndef SIM_HANDLE_H
#define SIM_HANDLE_H

#include "sim-macros.h"

SIM_BEGIN_DECLS

typedef struct _sim_handle_t sim_handle_t;

sim_handle_t  *sim_handle_create();
void           sim_handle_free(sim_handle_t *handle);
void           sim_handle_set_user_context(sim_handle_t *handle, void *);
void *         sim_handle_get_user_context(sim_handle_t *handle);

SIM_END_DECLS

#endif 
