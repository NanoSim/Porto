#include <gtest/gtest.h>
#include <Soft>

#ifndef PRINTF
#define PRINTF(...) \
  printf("[ OUTPUT   ]"); \
  printf(__VA_ARGS__); \
  printf("\n");
#endif


class BaseTest : public ::testing::Test {
protected:
  BaseTest() {
  }
  
  virtual ~BaseTest(){
  }
  
  virtual void SetUp(){
  }
  
  virtual void TearDown(){
  }
};

TEST_F(BaseTest, Foo) {
  int a = 3;
  EXPECT_EQ(a+a, 6);
}

TEST_F(BaseTest, uuidTest)
{
  std::string uuid = soft::uuidGen();
  EXPECT_EQ(uuid.length(), 36);
  PRINTF("%s %s %s", uuid.c_str(), "foo", "bla");
}


