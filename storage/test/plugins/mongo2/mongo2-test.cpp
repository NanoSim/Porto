#include <algorithm>
#include <gtest/gtest.h>
#include <Soft>
#include "chemkin_reaction.h"

TEST (MongoPluginTest2, checkRegistered)
{
  auto driver = soft::registeredStorageDrivers();
  auto it = std::find(driver.begin(), driver.end(), std::string("mongo2"));
  ASSERT_TRUE(it != driver.end());
}

TEST (MongoPluginTest2, storeData)
{
  auto storage = new soft::Storage("mongo2", "mongodb://localhost", "db=plugintest;coll=coll");
  ASSERT_TRUE(nullptr != storage);

  soft::Chemkin_reaction reaction(0, 0, 0, 0, 0);
  reaction.third_body = true;
  reaction.b = 1.0;
  reaction.Ea = 1.0E-2;
  reaction.A = 0.2;
  storage->save(&reaction);

  soft::Chemkin_reaction copy(reaction.id());
  storage->load(&copy);
  ASSERT_EQ(copy.third_body, reaction.third_body);
  ASSERT_DOUBLE_EQ(copy.b, reaction.b);
  ASSERT_DOUBLE_EQ(copy.Ea, reaction.Ea);
  ASSERT_DOUBLE_EQ(copy.A, reaction.A);
}
