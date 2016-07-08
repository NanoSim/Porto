#include <gtest/gtest.h>

static const char * driver  = "mongo";
static const char * uri     = "mongodb://localhost";
static const char * options = "db=soft;coll=testa";

class DataModelTest : public ::testing::Test {
protected:
  DataModelTest() {
  }

  virtual ~DataModelTest() {
  }

  virtual void SetUp(){
  }
  
  virtual void TearDown(){
  }
			  
};

TEST_F(DataModelTest, Foo)
{
  ASSERT_EQ(1, 2-1);
}
