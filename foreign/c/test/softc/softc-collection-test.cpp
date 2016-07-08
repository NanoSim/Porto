#include <gtest/gtest.h>
#include <softc/softc-utils.h>
#include <softc/softc-entity.h>
#include <softc/softc-collection.h>

#include "foo.h"

class CollectionTest : public ::testing::Test {
protected:
  static void SetUpTestCase() {}
  static void TearDownTestCase() {}      
};

TEST_F (CollectionTest, construct)
{
  softc_collection_s *coll = softc_collection_create(NULL);
  ASSERT_TRUE(coll != NULL);
  softc_collection_free(coll);
}


TEST_F (CollectionTest, store1)
{
  softc_collection_s *coll = softc_collection_create(NULL);
  foo_s *foo = foo_create(NULL, 10);
  foo_property_set_n(foo, 42);
  foo_property_set_str(foo, "Dette er en test");
  ASSERT_EQ(softc_collection_num_entities(coll), 0);
  ASSERT_EQ(softc_collection_num_relations(coll), 0);

  softc_collection_register_entity(coll, "foo", (const softc_entity_t*)foo);
  //ASSERT_EQ(softc_collection_num_entities(coll), 1);
  ASSERT_EQ(softc_collection_num_relations(coll), 0);

  softc_collection_free(coll);
  foo_free(foo);
}
