#include <gtest/gtest.h>
#include <softc/softc-utils.h>
#include <softc/softc-entity.h>
#include <softc/softc-collection.h>
#include <softc/softc-storage.h>

#include "foo.h"

class CollectionTest : public ::testing::Test {
protected:
  static void SetUpTestCase() {}
  static void TearDownTestCase() {}
};

TEST_F (CollectionTest, construct)
{
  softc_collection_s *coll = softc_collection_create(nullptr);
  ASSERT_TRUE(coll != nullptr);
  softc_collection_free(coll);
}


TEST_F (CollectionTest, store1)
{
  softc_collection_s *coll = softc_collection_create(nullptr);
  foo_s *foo = foo_create(nullptr, 10);
  foo_property_set_n(foo, 42);
  foo_property_set_str(foo, "Dette er en test");
  ASSERT_EQ(softc_collection_num_entities(coll), 0);
  ASSERT_EQ(softc_collection_num_relations(coll), 0);

  softc_collection_register_entity(coll, "foo", (softc_entity_t*)foo);
  //ASSERT_EQ(softc_collection_num_entities(coll), 1);
  ASSERT_TRUE(softc_collection_num_relations(coll) > 0);

  softc_collection_free(coll);
  foo_free(foo);
}

TEST_F (CollectionTest, setName)
{
  softc_collection_s *coll = softc_collection_create(nullptr);
  softc_collection_set_name(coll, "testname");
  softc_collection_set_version(coll, "TESTVERSION-1");

  softc_string_s s = softc_string_create("");
  softc_collection_get_name(coll, s);
  auto strComp = softc_string_compare(s, "testname");
  auto strCompFalse = softc_string_compare(s, "testname-false");
  ASSERT_EQ(strComp, 0);
  ASSERT_NE(strCompFalse, 0);

  softc_collection_get_version(coll, s);
  auto verComp = softc_string_compare(s, "TESTVERSION-1");
  auto verCompFalse = softc_string_compare(s, "TESTVERSION-1-FALSE");
  ASSERT_EQ(verComp, 0);
  ASSERT_NE(verCompFalse, 0);
  softc_string_destroy(s);
}

TEST_F(CollectionTest, addRelation)
{
  softc_collection_s *coll = softc_collection_create_new();
  softc_collection_add_relation(coll, "dad","loves","mommy");
  softc_collection_add_relation(coll, "history","repeats","itself");

  auto numRel = softc_collection_num_relations(coll);
  ASSERT_EQ(numRel, 4);
}

TEST_F(CollectionTest, findRelation)
{
  softc_collection_s *coll = softc_collection_create_new();
  softc_collection_add_relation(coll, "dad","loves","mommy");
  softc_collection_add_relation(coll, "history","repeats","itself");
  auto numRel = softc_collection_num_relations(coll);
  auto lst = softc_collection_find_relations(coll, "dad", "loves");

  ASSERT_STREQ(from_softc_string(softc_string_list_first(lst)), "mommy");
}

TEST_F (CollectionTest, store_and_load)
{
  auto storage = softc_storage_create("hdf5", "collection-test.h5", "");
  ASSERT_TRUE(storage != nullptr);
  softc_collection_s *coll = softc_collection_create_new();
  ASSERT_TRUE(coll != nullptr);

  softc_collection_add_relation(coll, "dad","loves","mommy");
  softc_collection_add_relation(coll, "history","repeats","itself");

  auto id = softc_entity_get_id((const softc_entity_t*)coll);

  softc_storage_save(storage, (const softc_entity_t*)coll);
  auto collection_copy = softc_collection_create (id);
  softc_storage_load(storage, (softc_entity_t*)collection_copy);

  auto numRel = softc_collection_num_relations(collection_copy);
  auto lst = softc_collection_find_relations(collection_copy, "dad", "loves");
  ASSERT_STREQ(from_softc_string(softc_string_list_first(lst)), "mommy");
}

TEST_F (CollectionTest, find_entity)
{
  softc_collection_s *coll = softc_collection_create(nullptr);
  foo_s *foo = foo_create(nullptr, 10);
  foo_property_set_n(foo, 42);
  softc_string_s name    = softc_string_create("");
  softc_string_s version = softc_string_create("");
  softc_string_s ns      = softc_string_create(""); 
  softc_string_s uuid    = softc_string_create("");
  foo_property_set_str(foo, "Dette er en test");
  auto en  = softc_entity_get_meta_name((const softc_entity_t*)foo);
  auto evs = softc_entity_get_meta_version((const softc_entity_t*)foo);
  auto ens = softc_entity_get_meta_namespace((const softc_entity_t*)foo);
  auto eid = softc_entity_get_id((const softc_entity_t*)foo);
  ASSERT_EQ(softc_collection_num_entities(coll), 0);
  ASSERT_EQ(softc_collection_num_relations(coll), 0);

  softc_collection_register_entity(coll, "foo", (softc_entity_t*)foo);
  ASSERT_EQ(softc_collection_num_entities(coll), 1);
  ASSERT_TRUE(softc_collection_num_relations(coll) > 0);

  softc_collection_find_entity(coll, "foo", name, version, ns, uuid);
  ASSERT_STREQ(from_softc_string(name), en);
  ASSERT_STREQ(from_softc_string(version), evs);
  ASSERT_STREQ(from_softc_string(ns), ens);
  ASSERT_STREQ(from_softc_string(uuid), eid);
  softc_collection_free(coll);
  foo_free(foo);
}

