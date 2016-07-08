#include <QtCore>
#include <gtest/gtest.h>
#include <soft.h>
#include <istoragestrategy.h>
#include <idatamodel.h>
#include <storage.h>
#include "testentity.h"

class HDF5StrategyTest : public ::testing::Test {
protected:
  static void SetUpTestCase()
  {
    storageStrategy_ = soft::createStrategy("hdf5", "empty.h5", "");
  }
  
  static void TearDownTestCase()
  {}

  static soft::IStorageStrategy *storageStrategy_;
};
soft::IStorageStrategy *HDF5StrategyTest::storageStrategy_ = nullptr;

TEST_F (HDF5StrategyTest, construct1)
{
  ASSERT_TRUE(storageStrategy_);
  ASSERT_STREQ(storageStrategy_->metaType(), "http://sintef.no/soft/TR/storage-strategy#hdf5:0.1-SNAPSHOT-1");
}

TEST_F (HDF5StrategyTest, write1)
{
  QString const filename = "test_write1.h5";
  auto storage = new soft::Storage("hdf5", qPrintable(filename), "");
  ASSERT_TRUE (nullptr != storage);

  TestEntity test;
  test.a = 42.0;
  std::vector<double> v = {1.0, 2.0, 3.0, 4.0, 5.0};  
  test.vec = v;
  test.text = "This is a test";
  storage->save(&test);

}
