#include <gtest/gtest.h>

static const char * driver  = "mongo";
static const char * uri     = "mongodb://localhost";
static const char * options = "db=soft;coll=testa";

class StorageStrategyTest : public ::testing::Test {
protected:
  StorageStrategyTest() {
  }

  virtual ~StorageStrategyTest() {
  }

};

TEST_F(StorageStrategyTest, Foo)
{
  ASSERT_EQ(1, 2-1);
}
