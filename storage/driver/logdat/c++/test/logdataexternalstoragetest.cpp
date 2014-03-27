#include <QtGlobal>
#include <QCoreApplication>
#include <gtest/gtest.h>
#include <externalstorage.h>
#include "logdataexternalstorage.h"

class LogDataExternalStorageTest : public ::testing::Test {
protected:
   virtual void SetUp() {
      if( QCoreApplication::instance() == 0 ) {
	 int argc = 1;
	 const char *argv[2] = { "portoapp", NULL };
	 app = new QCoreApplication(argc, const_cast<char**>(argv));
	 
	 ExternalStorage::registerExternalStorageDriver("LogData", new LogDataExternalStorage());
      }
   }
   
   virtual void TearDown() {
      delete app;
   }     

   QCoreApplication *app;
};

TEST_F(LogDataExternalStorageTest, addExternalStorageDriver)
{
   ExternalStorage storage = ExternalStorage::addExternalStorageDriver("LogData");
   ASSERT_TRUE(storage.driver() != NULL);
}
