#include <QtGlobal>
#include <QCoreApplication>
#include <gtest/gtest.h>
#include <externalstorage.h>
#include "mystorage.h"

class ExternalStorageTest : public ::testing::Test {
protected:
   virtual void SetUp() {
      if( QCoreApplication::instance() == 0 ) {
	 int argc = 1;
	 const char *argv[2] = { "softapp", NULL };
	 app = new QCoreApplication(argc, const_cast<char**>(argv));
	 
	 ExternalStorage::registerExternalStorageDriver("MyStorage", new MyStorage());
      }
   }
   
   virtual void TearDown() {
      delete app;
   }     

   QCoreApplication *app;
};

TEST_F(ExternalStorageTest, addExternalStorageDriver)
{
   ExternalStorage storage = ExternalStorage::addExternalStorageDriver("MyStorage");
   
}

TEST_F(ExternalStorageTest, readScalars)
{
   ExternalStorage storage = ExternalStorage::addExternalStorageDriver("MyStorage");
   auto driver = storage.driver();
   double pi;
   driver->readDouble("pi", &pi);
   EXPECT_DOUBLE_EQ(pi, 3.14);

   int mpl;
   driver->readInt("MPL", &mpl);
   EXPECT_EQ(mpl, 42);
}

TEST_F(ExternalStorageTest, readVectors)
{
  ExternalStorage storage = ExternalStorage::addExternalStorageDriver("MyStorage");
  auto driver = storage.driver();
  double *someContainer;
  const int rank = 2;
  const int dims[rank] = {3,3};
  someContainer = new double[9];
  bool isOk = driver->readDoubleArray("foobar", rank, dims, someContainer);
  ASSERT_TRUE(isOk);
}

TEST_F(ExternalStorageTest, nameAndVersion)
{
   ExternalStorage storage = ExternalStorage::addExternalStorageDriver("MyStorage");
   auto driver = storage.driver();
 
   ASSERT_TRUE(driver->name() == std::string("mystorage"));
   ASSERT_TRUE(driver->version() == std::string("0.0.1"));
   ASSERT_FALSE(driver->version() == driver->name());
}
