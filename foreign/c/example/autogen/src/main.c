#include <stdio.h>
#include <stdlib.h>
#include <softc/softc-entity.h>
#include <softc/softc-allocatable.h>
#include <softc/softc-storage.h>
#include <sim/sim.h>
#include "user_ctx.h"
#include "simple.h"
#include "demo.h"

void user_init    (sim_handle_t *handle, void *user_data);
void user_finalize(sim_handle_t *handle, void *user_data);

int main(int argc, char **argv)
{
  sim_application_t *sim_app;

  /* Initialize sim */
  sim_app = sim_init(&argc, argv);

  /* Register user defined functions*/ 
  sim_register_init     (sim_app, user_init);
  sim_register_compute  (sim_app, demo_compute);
  sim_register_finalize (sim_app, user_finalize);

  sim_run(sim_app, NULL);
  return 0;
}

void user_init (sim_handle_t *handle, void *user_data)
{
  user_ctx_s *user_ctx = malloc(sizeof *user_ctx);
  sim_handle_set_user_context(handle, user_ctx);
  const int ni = 3;
  const int nj = 2;
  int j, i;
  user_ctx->a = 42;
  user_ctx->simple = simple_create(ni,nj);

  double **A = simple_get_A(user_ctx->simple);
  double *x = simple_get_x(user_ctx->simple);

  A[0][0] = 1.0;
  A[0][1] = -1.0;
  A[0][2] = 2.0;
  A[1][0] = 0.0;
  A[1][1] = -3.0;
  A[1][2] = 1.0;

  x[0] = 2.0;
  x[1] = 1.0;
  x[2] = 0.0;
  
}

void user_finalize(sim_handle_t *handle, void *user_data)
{
  user_ctx_s *user_ctx = sim_handle_get_user_context(handle);
  softc_storage_t *storage  = softc_storage_create("hdf5", "demo.h5", "append=false");

  /* store the simple entity */
  softc_storage_save(storage, (softc_entity_t *)user_ctx->simple);

  /* print results to console */
  double *Ax = simple_get_Ax(user_ctx->simple);  
  printf("[%f, %f]\n", Ax[0], Ax[1]);

  /* free memory */
  softc_storage_free(storage);
  free(user_ctx);
  sim_handle_free(handle);
}
