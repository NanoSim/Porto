#include <QtCore>
#include <gtest/gtest.h>
#include <iostream>
#include <QMongo>
#include <softtypes.h>

using namespace soft;
TEST(QBsonTest, create)
{ 
  using bson::Bson;
  
  Bson bson;
  bson.appendString("test", "Dette er en streng");
  ASSERT_EQ(bson.countKeys(), 1);
  ASSERT_TRUE(bson.hasField("test"));
  ASSERT_FALSE(bson.hasField("should-not-exist"));
}

TEST(QBsonTest, appendString)
{
  using bson::Bson;
  Bson bson;
  bson.appendString("test", "Dette er en test");
  ASSERT_TRUE(bson.hasField("test"));
}

TEST(QBsonTest, appendStringList)
{
  using bson::Bson;
  Bson bson;
  QStringList xs = QStringList() << "this" << "is" << "a" << "test";
  bson.appendStringList("xlist", xs);
  ASSERT_TRUE(bson.hasField("xlist"));
}

TEST(QBsonTest, getStringList)
{
  using bson::Bson;
  Bson bson;
  QStringList xs = QStringList() << "this" << "is" << "a" << "test";
  bson.appendStringList("xlist", xs);
  ASSERT_TRUE(bson.hasField("xlist"));

  QStringList compare;
  bson.getStringList("xlist", compare);
  ASSERT_TRUE(compare.size() == 4);
  for (int i = 0; i < compare.size(); ++i) {
    ASSERT_STREQ(qPrintable(compare[i]), qPrintable(xs[i]));
  }
}

TEST(QBsonTest, getString)
{
  using bson::Bson;
  Bson bson;
  bson.appendString("test", "Dette er en test");
  ASSERT_TRUE(bson.hasField("test"));

  QString compare;
  bson.getString("test", compare);
  ASSERT_STREQ("Dette er en test", qPrintable(compare));
}

TEST(QBsonTest, appendInt32)
{
  using bson::Bson;
  Bson bson;
  bson.appendInt32("int32", 42);
  ASSERT_TRUE(bson.hasField("int32"));
}

TEST(QBsonTest, getInt32)
{
  using bson::Bson;
  Bson bson;
  bson.appendInt32("int32", 42);
  ASSERT_TRUE(bson.hasField("int32"));

  qint32 v;
  bson.getInt32("int32", v);
  ASSERT_EQ(v, 42);
}

TEST(QBsonTest, getInt64)
{
  using bson::Bson;
  Bson bson;
  bson.appendInt64("int64", 1.1529e+18);
  ASSERT_TRUE(bson.hasField("int64"));

  qint64 v;
  bson.getInt64("int64", v);
  ASSERT_EQ(v, 1.1529e+18);
}

TEST(QBsonTest, appendInt64)
{
  using bson::Bson;
  Bson bson;
  bson.appendInt32("int64", 42);
  ASSERT_TRUE(bson.hasField("int64"));
}

TEST(QBsonTest, appendDouble)
{
  using bson::Bson;
  Bson bson;
  bson.appendDouble("double", 3.14);
  ASSERT_TRUE(bson.hasField("double"));
}

TEST(QBsonTest, getDouble)
{
  using bson::Bson;
  Bson bson;
  bson.appendDouble("double", 3.14);
  ASSERT_TRUE(bson.hasField("double"));

  double compare;
  bson.getDouble("double", compare);
  ASSERT_DOUBLE_EQ(compare, 3.14);
  
}

TEST(QBsonTest, appendFloat)
{
  using bson::Bson;
  Bson bson;
  bson.appendFloat("float", 3.14f);
  ASSERT_TRUE(bson.hasField("float"));
}

TEST(QBsonTest, getFloat)
{
  using bson::Bson;
  Bson bson;
  bson.appendFloat("float", 3.14f);
  ASSERT_TRUE(bson.hasField("float"));

  float compare;
  bson.getFloat("float", compare);
  ASSERT_DOUBLE_EQ(3.14f, compare);
   
}

TEST(QBsonTest, appendBinary)
{
  using bson::Bson;
  Bson bson;
  QByteArray buffer;
  QDataStream stream(&buffer, QIODevice::WriteOnly);
  stream << 4.3 << "En test" << 42;

  bson.appendBinary("bin", buffer);    
}

TEST(QBsonTest, getBinary)
{
  using bson::Bson;
  Bson bson;
  QByteArray buffer;
  QDataStream stream(&buffer, QIODevice::WriteOnly);
  double vorig = 4.3;
  QString textorig("En test");
  int intorig = 42;
  stream << vorig << textorig << intorig;

  bson.appendBinary("bin", buffer);
  QByteArray copy;  
  bson.getBinary("bin", copy);
  QDataStream instream(&copy, QIODevice::ReadOnly);
  double v;
  QString text;
  int n;
  instream >> v >> text >> n;

  ASSERT_DOUBLE_EQ(v, vorig);
  ASSERT_STREQ(qPrintable(textorig), qPrintable(text));
  ASSERT_EQ(n, intorig);
}

TEST(QBsonTest, appendBson)
{
  using bson::Bson;

  Bson meta;
  Bson dims;
  Bson data;
  Bson doc;

  meta.appendString("name", "Foo");
  meta.appendString("version", "1.0");
  meta.appendString("namespace", "demo");
  dims.appendInt32("NI", 3);
  dims.appendInt32("NJ", 3);
  data.appendString("uri", "http://way.to.go");

  doc.appendBson("meta", meta);
  doc.appendBson("dims", dims);
  doc.appendBson("data", data);

  ASSERT_TRUE(doc.hasField("meta"));
  ASSERT_TRUE(doc.hasField("dims"));
  ASSERT_TRUE(doc.hasField("data"));
}

TEST(QBsonTest, appendIntArray)
{
  using bson::Bson;
  Bson bson;
  
  StdIntArray ints {8, 7, 6, 5, 4, 3, 2, 1};
  bson.append("int_array", ints);
  ASSERT_TRUE(bson.hasField("int_array"));

  StdIntArray copy;
  bson.get("int_array", copy);
  ASSERT_EQ(copy.size(), ints.size());
  for (int i = 0; i < copy.size(); ++i) {
    ASSERT_EQ(copy[i], ints[i]);
  }
}

TEST(QBsonTest, appendDoubleArray)
{
  using bson::Bson;
  Bson bson;
  
  StdDoubleArray i {8.1, 7.2, 6.3, 5.4, 4.5, 3.6, 2.7, 1.8};
  bson.append("double_array", i);
  ASSERT_TRUE(bson.hasField("double_array"));
}

TEST(QBsonTest, appendDoubleArray2D)
{
  using bson::Bson;
  Bson bson;
  
  StdDoubleArray2D ix(3);
  ix[0] = {8.1, 7.2, 6.3, 5.4, 4.5, 3.6, 2.7, 1.8};
  ix[1] = {4.1, 4.2, 3.3, 7.4, 4.5, 3.6, 2.7, 2.8};
  ix[2] = {5.1, 7.2, 7.3, 5.7, 4.5, 3.6, 2.7, 3.8};
  bson.append("double_array2d", ix);
  ASSERT_TRUE(bson.hasField("double_array2d"));

  StdDoubleArray2D copy;
  bson.get("double_array2d", copy);
  ASSERT_EQ(copy.size(), ix.size());
}

TEST(QBsonTest, appendDoubleArray3D)
{
  using bson::Bson;
  Bson bson;
  
  StdDoubleArray3D ix(3);
  StdDoubleArray2D id(1);
  id[0] = {1.0, 2.0, 3.0};
  ix[0] = id;
  bson.append("double_array3d", ix);
  ASSERT_TRUE(bson.hasField("double_array3d"));
}


TEST(QBsonTest, getDoubleArray3D)
{
  using bson::Bson;
  Bson bson;
  
  StdDoubleArray3D ix(3);
  StdDoubleArray2D id(1);
  id[0] = {1.0, 2.0, 3.0};
  ix[0] = id;
  bson.append("double_array3d", ix);
  ASSERT_TRUE(bson.hasField("double_array3d"));

  StdDoubleArray3D copy;
  bson.get("double_array3d", copy);
  ASSERT_EQ(copy, ix);
}

TEST(QBsonTest, keys)
{
  bson::Bson doc;
  doc.append("foo", "bar");
  doc.append("baz", "qux");
  doc.append("quux", "xyxxy");

  auto keys = doc.keys();
  ASSERT_EQ(keys.size(), 3);
  ASSERT_TRUE(keys.contains("foo"));
  ASSERT_TRUE(keys.contains("baz"));
  ASSERT_TRUE(keys.contains("quux"));
}

TEST(QBsonTest, getBson)
{
  bson::Bson doc, page1, page2;
  page1.appendString("test", "this is page 1");
  page2.appendString("test", "this is page 2");

  doc.appendBson("page1", page1);
  doc.appendBson("page2", page2);

  bson::Bson copy1, copy2;
  copy1 = doc.getBson("page1");
  copy2 = doc.getBson("page2");

  QString string;
  copy1.getString("test", string);
  ASSERT_STREQ(qPrintable(string), "this is page 1");
}

TEST(QBsonTest, appendInt)
{
  bson::Bson test;
  test.append("i", 20303);
}

TEST(QBsonTest, appendAndGet)
{
  StdInt i, i_copy;
  StdDouble d, d_copy;
  StdString str, str_copy;
  StdStringList strList, strList_copy;
  StdIntArray intArray, intArray_copy;
  StdDoubleArray dblArray, dblArray_copy;
  StdDoubleArray dblArray2D, dblArray2D_copy;
  StdDoubleArray dblArray3D, dblArray3D_copy;
  bson::Bson bson;
  bson.append("i", i);
  bson.append("d", d);
  bson.append("str", str);
  bson.append("strList", strList);
  bson.append("intArray", intArray);
  bson.append("dblArray", dblArray);
  bson.append("dblArray2D", dblArray2D);
  bson.append("dblArray2D", dblArray3D);

  bson.get("i", i_copy);
  bson.get("d", d_copy);
  bson.get("str", str_copy);
  bson.get("strList", strList_copy);
  bson.get("intArray", intArray_copy);
  bson.get("dblArray", dblArray_copy);
  bson.get("dblArray2D", dblArray2D_copy);
  bson.get("dblArray2D", dblArray3D_copy);

}
