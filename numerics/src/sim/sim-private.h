#ifndef SIM_PRIVATE_H_DEF
#define SIM_PRIVATE_H_DEF

#include "sim-handle.h"

struct _sim_application_t
{
  sim_handle_t *ctx;
  void (*user_init)        (sim_handle_t*, void*);
  void (*user_pre_init)    (sim_handle_t*, void*);
  void (*user_post_init)   (sim_handle_t*, void*);
  void (*user_compute)     (sim_handle_t*, void*);
  void (*user_begin_iter)  (sim_handle_t*, void*);
  void (*user_end_iter)    (sim_handle_t*, void*);
  void (*user_finalize)    (sim_handle_t*, void*);
  void (*user_pre_finalize)(sim_handle_t*, void*);
  void (*user_post_finalize)(sim_handle_t*, void*);
  void (*user_time_stepper)(sim_handle_t*, void (*)(sim_handle_t*, void*), void (*)(sim_handle_t*, void*), void (*)(sim_handle_t*, void*), void*);
  void *libraryMap;
};

#endif
