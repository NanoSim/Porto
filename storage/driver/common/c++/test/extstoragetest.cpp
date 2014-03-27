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
	 const char *argv[2] = { "portoapp", NULL };
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
