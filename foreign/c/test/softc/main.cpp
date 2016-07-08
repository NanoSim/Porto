#include <gtest/gtest.h>
#include <softc/softc.h>

int main(int argc, char **argv) {
  softc_t *app = softc_init(argc, argv);
  ::testing::InitGoogleTest(&argc, argv);

  int result = RUN_ALL_TESTS();
  softc_cleanup(app);
  return result;
}
