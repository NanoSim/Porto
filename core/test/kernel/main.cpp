#include <gtest/gtest.h>
#include <Soft>

int main(int argc, char **argv) {
  ::soft::init(argc, argv);
  ::testing::InitGoogleTest(&argc, argv);

  for (auto driver : ::soft::registeredStorageDrivers()) {
    printf("driver: %s\n", driver.c_str());
  }
  return RUN_ALL_TESTS();
}
