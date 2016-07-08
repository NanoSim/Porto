#include <gtest/gtest.h>
#include <QtCore>
#include <soft.h>
#include <storage.h>
#include "testentity.h"

class StorageTest : public ::testing::Test {
protected:
  StorageTest()
  {}

  virtual ~StorageTest()
  {}
};


TEST_F(StorageTest, construct1)
{
  auto storage = new soft::Storage("json", "storagetest.json", "");
  ASSERT_TRUE(nullptr != storage);
}
  
TEST_F(StorageTest, construct2)
{
  QString const filename = "storagetest.json";
  auto storage = new soft::Storage("json", qPrintable(filename), "");
  
  ASSERT_TRUE(nullptr != storage);
  
  TestEntity test;
  test.a = 42.0;
  std::vector<double> v = {1.0, 2.0, 3.0, 4.0, 5.0};  
  test.vec = v;
  test.text = "This is a test";
  storage->save(&test);
  
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    ASSERT_TRUE(false);
  }
  auto json = file.readAll();
  QJsonDocument vfy = QJsonDocument::fromJson(json);
  QJsonObject obj = vfy.object();
  QString val = obj.value("text").toString();
  ASSERT_TRUE(val == "This is a test");    
}

TEST_F(StorageTest, construct3)
{
  QString const filename = "storagetest.json";
  auto storage = new soft::Storage("json", qPrintable(filename), "");

  TestEntity test;
  storage->load(&test);

  ASSERT_DOUBLE_EQ(test.a, 42.0);
  ASSERT_STREQ(test.text.c_str(), "This is a test");
  ASSERT_DOUBLE_EQ(test.vec[0], 1.0);
  ASSERT_DOUBLE_EQ(test.vec[1], 2.0);
  ASSERT_DOUBLE_EQ(test.vec[2], 3.0);
  ASSERT_DOUBLE_EQ(test.vec[3], 4.0);
  ASSERT_DOUBLE_EQ(test.vec[4], 5.0);
  
}
