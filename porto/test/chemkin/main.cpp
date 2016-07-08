#include <gtest/gtest.h>
#include <Soft>

int main(int argc, char **argv) {
  ::soft::init(argc, argv);

  for (auto driver : ::soft::registeredStorageDrivers()) {
    printf("driver: %s\n", driver.c_str());
  }

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
