#include <memory>
#include <gtest/gtest.h>
#include <soft.h>
#include <collection.h>
#include <istoragestrategy.h>
#include <idatamodel.h>
#include <json/jsonmodel.h>
#include "collection_test_entity.h"
#include <softc/softc-entity.h>

//static const char * driver  = "mongo";
static const char * driver = "json";
static const char * uri    = "output.json";
static const char * options= "";

//static const char * uri     = "mongodb://localhost";
//static const char * options = "db=soft;coll=testa";

class CollectionTest : public ::testing::Test {
protected:
  CollectionTest() {
  }

  virtual ~CollectionTest(){
  }

  static void SetUpTestCase() {
    printf("SetUpTestCase");
    storageStrategy_ = soft::createStrategy(driver, uri, options);
    printf("%s\n", storageStrategy_->metaType());
  }

  static void TearDownTestCase() {
    delete storageStrategy_;
    storageStrategy_ = nullptr;
  }

  virtual void SetUp(){
  }

  virtual void TearDown(){
  }

  static soft::IStorageStrategy *storageStrategy_;
};

soft::IStorageStrategy *CollectionTest::storageStrategy_ = nullptr;

TEST_F(CollectionTest, Constructor1) {
  soft::Collection *coll = new soft::Collection();
  ASSERT_TRUE(NULL != coll);
}

TEST_F(CollectionTest, Constructor2) {
  const char *uuid = "6b3827f8-1386-481d-a6cb-396e07575666";
  soft::Collection *coll = new soft::Collection(uuid);
  ASSERT_TRUE(NULL != coll);
  delete coll;
}

TEST_F(CollectionTest, Constructor3) {
  soft::Collection *orig = new soft::Collection();
  soft::Collection *coll = new soft::Collection(orig);
  ASSERT_TRUE(NULL != coll);
  delete coll;
  delete orig;
}

TEST_F(CollectionTest, StaticCreate) {
  const char *uuid = "88bf2784-5939-42e9-8b95-2626d21b1c07";
  soft::Collection *coll = (soft::Collection*)soft::Collection::create(uuid);
  ASSERT_TRUE(NULL != coll);
  delete coll;
}

TEST_F(CollectionTest, createAsEntity) {
  soft::IEntity *e = soft::Collection::create();
  ASSERT_TRUE(e != NULL);
  delete e;
}


TEST_F(CollectionTest, registerEntity) {
  soft::IEntity *e = soft::Collection::create();
  ASSERT_TRUE(e != NULL);

  soft::Collection collection;
  collection.registerEntity("label1", e);
  ASSERT_EQ(1, collection.numEntities());
}

TEST_F(CollectionTest, collection) {
  soft::Collection collection;
  collection.addRelation("label1", "inherits", "label2");
  collection.addRelation("label2", "inherits", "label3");
}

TEST_F(CollectionTest, DISABLED_registerAndFetch) {
  soft::Collection subCollection;
  soft::Collection collection;
  collection.registerEntity("subCollection", &subCollection);
  collection.connect("subCollection", "is", "subCollection");

  soft::IEntity const *e = collection.findInstance("subCollection");
  ASSERT_TRUE(e != NULL);
  ASSERT_STREQ(e->id().c_str(), subCollection.id().c_str());
}

TEST(Collection, instanciateFromDataModel) {

  soft::Collection recepie;
  soft::Collection baking_log;

  soft::Collection mums_cookies;

  // Confirm that we have been able to populate the collection.
  mums_cookies.setName("Mums best cookies!");
  mums_cookies.setVersion("1-with-some-improvements");
  ASSERT_EQ("Mums best cookies!", mums_cookies.name());
  ASSERT_EQ("1-with-some-improvements", mums_cookies.version());

  mums_cookies.registerEntity("cookie-recepie", &recepie);
  mums_cookies.registerEntity("cooking-competition-baking-log", &baking_log);
  ASSERT_EQ(2, mums_cookies.numEntities()) << "mums_cookies numEntities";

  mums_cookies.addRelation("cookie-recepie", "loged-in", "cooking-competition-baking-log");
  // TODO: Relations not yet implemented
  // ASSERT_EQ(1, mums_cookies.numRelations());

  // TODO: Can this fail?
  soft::JSONModel dm;
  mums_cookies.save((soft::IDataModel *)(&dm));

  // ... mum shelves her cookie activities and pursue other activities
  // while a generation passes. Until one day ...

  soft::Collection grandmas_cookies;
  grandmas_cookies.load(&dm);

  // Confirm that what we retrieve from the collection through the
  // data model is exactly what we sent in.
  ASSERT_EQ("Mums best cookies!", grandmas_cookies.name());
  ASSERT_EQ("1-with-some-improvements", grandmas_cookies.version());

  // TODO: Relations not yet implemented
  // ASSERT_EQ(1, grandmas_cookies.numRelations());

  ASSERT_EQ(2, grandmas_cookies.numEntities()) << "grandmas_cookies numEntities";


  // TODO: Check that I got a label here
}

TEST_F(CollectionTest, saveAndLoadWithEntities) {
  soft::Collection c;
  soft::Collection e;

  e.setName("My-sub-entity");
  e.setVersion("1.0");
  c.setName("My-collection");
  c.setVersion("2.0");

  c.attachEntity("sub-entity", &e);

  soft::JSONModel dm;
  c.save(&dm);

  soft::Collection e2;
  soft::Collection c2;
  c2.attachEntity("sub-entity", &e2);

  c2.load(&dm);

  ASSERT_EQ("My-collection", c2.name());
  ASSERT_EQ("2.0", c2.version());
  ASSERT_EQ("My-sub-entity", e2.name());
  ASSERT_EQ("1.0", e2.version());
  ASSERT_EQ(e.id(), e2.id());
}

TEST_F(CollectionTest, findEntity)
{
  soft::Collection c2;
  c2.addEntity("lbl1", "foo", "1.0", "space", "647ab24d-2904-4f5d-9d37-581cd3f72367");
  c2.addEntity("lbl2", "bar", "2.0", "face", "245f8dca-4d42-4d3c-8c5d-8099b6b914d8");
  c2.addEntity("lbl3", "baz", "1.0", "home", "d1f68e22-919d-44bc-a915-ad3436342b0a");
  c2.addEntity("lbl4", "qnx", "1.0", "net", "c2b39d37-9f5b-4bba-9040-842465aa5af8");

  std::string name, version, ns, uuid;
  c2.findEntity("lbl1", name, version, ns, uuid);
  ASSERT_STREQ(name.c_str(), "foo");
  ASSERT_STREQ(version.c_str(), "1.0");
  ASSERT_STREQ(ns.c_str(), "space");
  ASSERT_STREQ(uuid.c_str(), "647ab24d-2904-4f5d-9d37-581cd3f72367");

  c2.findEntity("lbl3", name, version, ns, uuid);
  ASSERT_STREQ(name.c_str(), "baz");
  ASSERT_STREQ(version.c_str(), "1.0");
  ASSERT_STREQ(ns.c_str(), "home");
  ASSERT_STREQ(uuid.c_str(), "d1f68e22-919d-44bc-a915-ad3436342b0a");
}
