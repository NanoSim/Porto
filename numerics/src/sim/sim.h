#ifndef SIM_H_DEF
#define SIM_H_DEF

#include "sim-macros.h"
#include "sim-handle.h"

SIM_BEGIN_DECLS

typedef struct _sim_application_t sim_application_t;

sim_application_t* sim_init(int *argc, char **argv);
void               sim_load                  (sim_application_t *);
void               sim_register_init         (sim_application_t *, void(*)(sim_handle_t*, void*));
void               sim_register_pre_init     (sim_application_t *, void(*)(sim_handle_t*, void*));
void               sim_register_post_init    (sim_application_t *, void(*)(sim_handle_t*, void*));
void               sim_register_compute      (sim_application_t *, void(*)(sim_handle_t*, void*));
void               sim_register_time_stepper (sim_application_t *,
					      void(*fn)(sim_handle_t*, void(*)(sim_handle_t*, void*),
							void(*)(sim_handle_t*, void*),
							void(*)(sim_handle_t*, void*),
							void*));
void               sim_register_begin_iter   (sim_application_t *, void(*)(sim_handle_t*, void*));
void               sim_register_end_iter     (sim_application_t *, void(*)(sim_handle_t*, void*));
void               sim_register_finalize     (sim_application_t *, void(*)(sim_handle_t*, void*));
void               sim_register_pre_finalize (sim_application_t *, void(*)(sim_handle_t*, void*));
void               sim_register_post_finalize(sim_application_t *, void(*)(sim_handle_t*, void*));
void               sim_run                   (sim_application_t *, void*);
void               sim_register_toolkit      (sim_application_t *, const char*);

SIM_END_DECLS
#endif /* SIM_H_DEF */
