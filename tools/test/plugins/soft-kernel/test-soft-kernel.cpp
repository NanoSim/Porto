#include <gtest/gtest.h>
#include <QtCore>
#include <Soft>
#include "qcollection.h"
#include "qgenericentity.h"

TEST(SoftKernelPluginTest, Construct)
{
  auto coll = new QCollection();
  auto id = coll->id();
  QUuid uuid(id);
  ASSERT_TRUE(!uuid.isNull());
}

TEST(SoftKernelPluginTest, Construct2)
{
  auto coll = new QGenericEntity();
  auto id = coll->id();
  QUuid uuid(id);
  ASSERT_TRUE(!uuid.isNull());
}
