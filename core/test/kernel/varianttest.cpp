#include <string>
#include <vector>
#include <list>
#include <gtest/gtest.h>
#include <variant.h>
#include <softtypes.h>

using StdString = std::string;
using StdVector = std::vector<double>;
using StdInt    = int;
using StdDouble = double;
using StdVar    = soft::Variant<StdString, StdInt, StdDouble, StdVector>;

class VariantTest : public ::testing::Test {
protected:
  VariantTest() {}
  virtual ~VariantTest() {}

  soft::StdVariant myVariant;
};

using StdString = std::string;
using StdVector = std::vector<double>;
using StdInt    = int;
using StdDouble = double;
using StdVar    = soft::Variant<StdString, StdInt, StdDouble, StdVector>;

TEST_F(VariantTest, asString)
{
  StdVar d;

  d.set<StdString>("test");
  ASSERT_STREQ(d.get<StdString>().c_str(), "test");
}

TEST_F(VariantTest, asInteger)
{ 
  StdVar d;

  d.set<StdInt>(42);
  ASSERT_EQ(d.get<StdInt>(), 42);
}

TEST_F(VariantTest, asDouble)
{
  StdVar d;
  d.set<StdDouble>(3.14);
  EXPECT_DOUBLE_EQ(d.get<StdDouble>(), 3.14);
}

TEST_F(VariantTest, asVector)
{
  StdVar d;
  d.set<StdVector>(StdVector({1.0,2.0,3.0,4.0}));
  EXPECT_DOUBLE_EQ(d.get<StdVector>()[0], 1.0);
  EXPECT_DOUBLE_EQ(d.get<StdVector>()[1], 2.0);
  EXPECT_DOUBLE_EQ(d.get<StdVector>()[2], 3.0);
  EXPECT_DOUBLE_EQ(d.get<StdVector>()[3], 4.0);
}

TEST_F(VariantTest, is)
{
  StdVar d;
  d.set<StdString>("Test");
  ASSERT_TRUE(d.is<StdString>());
  ASSERT_FALSE(d.is<int>());

  d.set<StdInt>(42);
  ASSERT_TRUE(d.is<int>());
  ASSERT_FALSE(d.is<StdString>());
}

TEST_F(VariantTest, valid)
{
  using FooVar = soft::Variant<int>;
  FooVar foo;
  foo.set<int>(42);
  ASSERT_TRUE(foo.valid());

  FooVar n;
  ASSERT_FALSE(n.valid());
}

TEST_F(VariantTest, blob)
{
  using Blob = std::vector<unsigned char>;
  using MyVar = soft::Variant<Blob>;
  MyVar var;
  Blob data = {'a','C','b'};
  var.set<Blob>(data);
  ASSERT_EQ(var.get<Blob>()[0], data[0]);
  ASSERT_EQ(var.get<Blob>()[1], data[1]);  
  ASSERT_EQ(var.get<Blob>()[2], data[2]);  
}

TEST_F(VariantTest, types)
{
  soft::StdString str("en test");
  soft::StdVariant variant;
  variant.set<StdString>(str);
}

TEST_F(VariantTest, stdVariantTest)
{
  myVariant.set<soft::StdString>("dette er en test");
  ASSERT_STREQ(myVariant.get<soft::StdString>().c_str(), "dette er en test");

  soft::StdIntArray array({1,2,3,4,5,6,7,8});
  myVariant.set<soft::StdIntArray>(array);
  ASSERT_EQ(myVariant.get<soft::StdIntArray>()[0], 1);
}

TEST_F(VariantTest, typeId)
{
  myVariant.set<soft::StdString>("en test");
  ASSERT_EQ(myVariant.type(), soft::VariantType<soft::StdString>::value);
  ASSERT_EQ(myVariant.type(), soft::StdTypes::String);

  auto vtype = [](soft::StdVariant &variant) -> std::string{
    switch(variant.type()) {
    case soft::StdTypes::String:
      return std::string("string");      
    case soft::StdTypes::Int:
      return std::string("int");
    }
  };

  ASSERT_STREQ(vtype(myVariant).c_str(), "string");
}


int passAsConst(soft::StdVariant const &v)
{
  if (v.type() == soft::StdTypes::Int) {
    return v.get<StdInt>();
  }
  return -1;
}

TEST_F(VariantTest, passingArg)
{
  myVariant.set<soft::StdInt>(3);
  ASSERT_EQ(3, passAsConst(myVariant));
}

TEST_F(VariantTest, toInt)
{
  myVariant.set<soft::StdUInt8>(32);
  ASSERT_EQ(32, myVariant.toInt());
}

