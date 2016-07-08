#include <cstdint>
#include <algorithm>
#include <gtest/gtest.h>
#include <Soft>

class Hdf5PluginTest : public ::testing::Test {
protected:
  Hdf5PluginTest()
    : id( "bb89e8a3-b624-400b-8150-433d7bb6398f")
    , i8( -10)
    , ui8( 10)
    , i16( -3000)
    , ui16( 10)
    , f( 3.14f)
    , d( 1.1e-10)
    , yes( true)
    , no( !yes)
    , text( "This is a text")
    , intVec({1,2,3,4,5,6,7})
    , doubleVec({1.1, 2.2, 3.3, 4.4, 5.5, 6.6})
    , doubleVec2D({{1.0, 1.1, 1.2},
	  {2.0, 2.1, 2.2},
	    {3.0, 3.1, 3.2}})
    , doubleVec3D(
		  {{{1.0, 0.1, 0.2},{0.0, 1.1, 0.2},{3.0, 3.1, 3.2}},
		      {{1.0, 1.1, 1.2},{2.0, 2.1, 2.2},{3.0, 3.1, 3.2}},
            {{1.0, 1.1, 4.2},{2.0, 2.1, 3.2},{3.0, 3.1, 3.2}}})
    , stringVec({"This", "is", "a", "test"})
  {}
  
  static void SetUpTestCase()
  {}
  
  static void TearDownTestCase()
  {}

  int8_t i8;
  uint8_t ui8;
  int16_t i16;
  uint16_t ui16;
  float f;
  double d;
  bool yes;
  bool no;
  std::string text;
  std::vector<int32_t> intVec;
  std::vector<double> doubleVec;
  std::vector<std::vector<double> > doubleVec2D;
  std::vector<std::vector<std::vector<double> > > doubleVec3D;
  std::vector<std::string> stringVec;
  std::string id;
};

TEST_F(Hdf5PluginTest, checkRegistered)
{
  auto driver = soft::registeredStorageDrivers();
  auto it = std::find(driver.begin(), driver.end(), std::string("hdf5"));
  ASSERT_TRUE(it != driver.end());
}

TEST_F(Hdf5PluginTest, writeTest)
{
  soft::Storage storage("hdf5", "test_f.h5");

  auto strategy = storage.strategy();
  ASSERT_TRUE(nullptr != strategy);
  
  auto model    = strategy->dataModel();
  ASSERT_TRUE(nullptr != model); 
  
  model->setId(id);
  model->setMetaName("foo");
  model->setMetaVersion("foo");
  model->setMetaNamespace("foo");
  
  model->appendInt8("i8", i8);
  model->appendUInt8("ui8", ui8);
  model->appendInt16("i16", i16);
  model->appendUInt16("ui16", ui16);
  model->appendString("text", text);
  model->appendFloat("f", f);
  model->appendDouble("d", d);
  model->appendBool("yes", yes);
  model->appendBool("no", no);
  model->appendInt32Array("intArray", intVec);
  model->appendDoubleArray("doubleArray", doubleVec);
  model->appendDoubleArray2D("doubleArray2D", doubleVec2D);
  model->appendDoubleArray3D("doubleArray3D", doubleVec3D);
  model->appendStringArray("stringList", stringVec);
  strategy->store(model);
}

TEST_F(Hdf5PluginTest, readInt)
{
  int8_t compare_i8;
  uint8_t compare_ui8;
  int16_t compare_i16;
  uint16_t compare_ui16;

  soft::Storage storage("hdf5", "test_f.h5");
  
  auto strategy = storage.strategy();
  ASSERT_TRUE(nullptr != strategy);
  
  auto model    = strategy->dataModel();
  ASSERT_TRUE(nullptr != model);

  model->setId(id);
  strategy->startRetrieve(model);
  model->getInt8("i8", compare_i8);
  model->getUInt8("ui8", compare_ui8);
  model->getInt16("i16", compare_i16);
  model->getUInt16("ui16", compare_ui16);
  strategy->endRetrieve(model);
  
  ASSERT_EQ(i8, compare_i8);
  ASSERT_EQ(ui8, compare_ui8);
  ASSERT_EQ(i16, compare_i16);
  ASSERT_EQ(ui16, compare_ui16);
}

TEST_F(Hdf5PluginTest, readFloat)
{
  float compare;
  soft::Storage storage("hdf5", "test_f.h5");
  
  auto strategy = storage.strategy();
  auto model    = strategy->dataModel();
  model->setId(id);
  strategy->startRetrieve(model);

  model->getFloat("f", compare);
  strategy->endRetrieve(model);
  ASSERT_EQ(compare, f);
}

TEST_F(Hdf5PluginTest, readDouble)
{
  double compare;
  soft::Storage storage("hdf5", "test_f.h5");
  
  auto strategy = storage.strategy();
  auto model    = strategy->dataModel();
  model->setId(id);
  strategy->startRetrieve(model);

  model->getDouble("d", compare);
  strategy->endRetrieve(model);
  ASSERT_EQ(compare, d);
}

TEST_F(Hdf5PluginTest, readBool)
{
  bool y,n;
  soft::Storage storage("hdf5", "test_f.h5");
  
  auto strategy = storage.strategy();
  auto model    = strategy->dataModel();
  model->setId(id);
  strategy->startRetrieve(model);

  model->getBool("yes", y);
  model->getBool("no", n);
  strategy->endRetrieve(model);
  ASSERT_TRUE(y);
  ASSERT_FALSE(n);
}


TEST_F(Hdf5PluginTest, readString)
{
  std::string compare_text;

  soft::Storage storage("hdf5", "test_f.h5");
  auto strategy = storage.strategy();
  ASSERT_TRUE(nullptr != strategy);
  
  auto model    = strategy->dataModel();
  ASSERT_TRUE(nullptr != model);

  model->setId(id);
  strategy->startRetrieve(model);
  model->getString("text", compare_text);
  strategy->endRetrieve(model);

  ASSERT_STREQ(text.c_str(), compare_text.c_str());
}

TEST_F(Hdf5PluginTest, readIntArray)
{
  std::vector<int32_t> compare;
  soft::Storage storage("hdf5", "test_f.h5");
  auto strategy = storage.strategy();
  ASSERT_TRUE(nullptr != strategy);
  
  auto model    = strategy->dataModel();
  ASSERT_TRUE(nullptr != model);
  
  model->setId(id);
  strategy->startRetrieve(model);
  model->getInt32Array("intArray", compare);
  strategy->endRetrieve(model);

  ASSERT_TRUE(compare.size() == intVec.size());
  for (int i = 0; i < intVec.size(); ++i) {
    ASSERT_EQ(compare[i], intVec[i]);
  }
}


TEST_F(Hdf5PluginTest, readDoubleArray)
{
  std::vector<double> compare;
  soft::Storage storage("hdf5", "test_f.h5");
  auto strategy = storage.strategy();
  ASSERT_TRUE(nullptr != strategy);
  
  auto model    = strategy->dataModel();
  ASSERT_TRUE(nullptr != model);
  
  model->setId(id);
  strategy->startRetrieve(model);
  model->getDoubleArray("doubleArray", compare);
  strategy->endRetrieve(model);

  ASSERT_TRUE(compare.size() == doubleVec.size());
  for (int i = 0; i < doubleVec.size(); ++i) {
    ASSERT_EQ(compare[i], doubleVec[i]);
  }
}

TEST_F(Hdf5PluginTest, readDoubleArray2D)
{
  std::vector<std::vector<double> > compare;
  soft::Storage storage("hdf5", "test_f.h5");
  auto strategy = storage.strategy();
  ASSERT_TRUE(nullptr != strategy);
  
  auto model    = strategy->dataModel();
  ASSERT_TRUE(nullptr != model);
  
  model->setId(id);
  strategy->startRetrieve(model);
  model->getDoubleArray2D("doubleArray2D", compare);
  strategy->endRetrieve(model);

  ASSERT_TRUE(compare == doubleVec2D);
}

TEST_F(Hdf5PluginTest, readDoubleArray3D)
{
  std::vector<std::vector<std::vector<double> > > compare;
  soft::Storage storage("hdf5", "test_f.h5");
  auto strategy = storage.strategy();
  ASSERT_TRUE(nullptr != strategy);
  
  auto model    = strategy->dataModel();
  ASSERT_TRUE(nullptr != model);
  
  model->setId(id);
  strategy->startRetrieve(model);
  model->getDoubleArray3D("doubleArray3D", compare);
  strategy->endRetrieve(model);

  ASSERT_TRUE(compare == doubleVec3D);
}

TEST_F(Hdf5PluginTest, readStringList)
{
    std::vector<std::string> compare;
    soft::Storage storage("hdf5", "test_f.h5");
    auto strategy = storage.strategy();
    ASSERT_TRUE(nullptr != strategy);

    auto model    = strategy->dataModel();
    ASSERT_TRUE(nullptr != model);

    model->setId(id);
    strategy->startRetrieve(model);
    model->getStringArray("stringList", compare);
    strategy->endRetrieve(model);

    ASSERT_TRUE(compare == stringVec);
}

