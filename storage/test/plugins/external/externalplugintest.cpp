#include <list>
#include <algorithm>
#include <gtest/gtest.h>
#include <Soft>
#include <softc/softc-entity.h>
#include <softc/softc-allocatable.h>
#include <softc/softc-storage.h>
#include "xy.h"
#include "amount.h"

class ExternalPluginTest : public ::testing::Test {
protected:
  ExternalPluginTest()
{}
  static void SetUpTestCase()
  {}
  static void TearDownTestCase()
  {}  
};


TEST_F(ExternalPluginTest, checkRegistered)
{
  auto driver = soft::registeredStorageDrivers();
  auto it = std::find(driver.begin(), driver.end(), std::string("external"));
  ASSERT_TRUE(it != driver.end());
}

TEST_F(ExternalPluginTest, connect)
{  
  softc_storage_t *storage  = softc_storage_create("external", "porto-json:///tmp/file.json", "");
  xy_s * xy = xy_create(3,2);
  amount_s *a = amount_create();

  softc_storage_save(storage, (softc_entity_t*)a);
  ASSERT_TRUE(storage != nullptr);
}
