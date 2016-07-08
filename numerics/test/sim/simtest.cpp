#include <gtest/gtest.h>
#include <sim/sim.h>

#include "args.h"

class SimTest : public ::testing::Test {
protected:
  static void SetUpTestCase() {}
  static void TearDownTestCase() {}
};

struct counter_s{
  int idx;
};

static struct counter_s cnt;

void init(sim_handle_t *h, void *user_data)
{
  struct counter_s *data = (struct counter_s *)user_data;
  data->idx++;
}

void compute(sim_handle_t *h, void *user_data)
{
  struct counter_s *data = (struct counter_s *)user_data;
  data->idx++;
}

void finalize(sim_handle_t *h, void *user_data)
{
  struct counter_s *data = (struct counter_s *)user_data;
  data->idx++;
}


TEST_F (SimTest, construct)
{  
  sim_application_t* app = sim_init(&t_argc, t_argv);
  ASSERT_TRUE(app != NULL);

  cnt.idx = 0;
  sim_register_init(app, init);
  sim_register_compute(app, compute);
  sim_register_finalize(app, finalize);
  
  sim_run(app, (void*)(&cnt));
  ASSERT_EQ(cnt.idx, 3);
}
