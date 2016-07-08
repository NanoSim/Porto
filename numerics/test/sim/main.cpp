#include <gtest/gtest.h>
#include <Soft>
#include "args.h"


int t_argc;
char **t_argv;

int main(int argc, char **argv) {
  ::soft::init(argc, argv);
  ::testing::InitGoogleTest(&argc, argv);
  t_argc = argc;
  t_argv = argv;
  return RUN_ALL_TESTS();
}
