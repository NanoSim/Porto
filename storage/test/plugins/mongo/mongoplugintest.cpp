#include <QtCore>
#include <algorithm>
#include <gtest/gtest.h>
#include <Soft>
#include <qbson.h>
#include "testentity.h"

class MongoPluginTest : public ::testing::Test {
protected:
  static void SetUpTestCase()
  {}
  
  static void TearDownTestCase()
  {}
};

TEST_F(MongoPluginTest, checkRegistered)
{
  auto driver = soft::registeredStorageDrivers();
  auto it = std::find(driver.begin(), driver.end(), std::string("mongo"));
  ASSERT_TRUE(it != driver.end());
}

TEST_F(MongoPluginTest, storeData)
{
  auto storage = new soft::Storage("mongo", "mongodb://localhost", "db=test;coll=coll");
  ASSERT_TRUE (nullptr != storage);
  TestEntity test;
  test.text = "Test";
  test.a = 3.14;
  storage->save(&test);

  TestEntity cpy;
  storage->load(&cpy);
}

TEST_F(MongoPluginTest, QBson_test)
{
  soft::bson::Bson document;
  soft::bson::Bson entity;
  soft::bson::Bson properties;
  soft::bson::Bson meta;
  soft::bson::Bson dimensions;
  dimensions.appendInt32("NI", 3);
  dimensions.appendInt32("NJ", 3);
  meta.appendString("name", "Foo");
  meta.appendString("version", "0.1-SNAPSHOT-1");
  meta.appendString("namespace", "demo");
  properties.appendString("name", "Something");
  entity.appendBson("properties",properties);
  entity.appendBson("meta",meta);
  entity.appendBson("dimensions",dimensions);
  document.appendBson("671d0a9c-0d71-426d-8d33-a85c8f3321f9", entity);
  QTextStream(stdout) << document.asString();
}
