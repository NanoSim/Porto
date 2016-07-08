#include <gtest/gtest.h>
#include <Soft>

int main(int argc, char **argv) {
  ::soft::init(argc, argv);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
