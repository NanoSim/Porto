#include <stdlib.h>
#include "sim-handle.h"

struct _sim_handle_t
{
  void *user_context;
};

sim_handle_t *sim_handle_create()
{
  sim_handle_t *handle;
  handle = (sim_handle_t*)malloc(sizeof (*handle));
  handle->user_context = NULL;
  return handle;
}

void sim_handle_free(sim_handle_t *handle)
{
  free (handle);
}

void sim_handle_set_user_context(sim_handle_t *handle, void *data)
{
  handle->user_context = data;
}

void * sim_handle_get_user_context(sim_handle_t *handle)
{
  return handle->user_context;
}
