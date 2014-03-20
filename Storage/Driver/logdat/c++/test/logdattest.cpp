#include <QtGlobal>
#include <gtest/gtest.h>
#include <logdatareader.h>

class LogDataReaderTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    LogDataReader const reader("test_input_0.logDat");
    log = reader.readLogData();  
  }

  virtual void TearDown() 
  {}

  LogData log;
};

TEST_F( LogDataReaderTest, Processor)
{
  EXPECT_EQ(log.processor, 0);
}

TEST_F( LogDataReaderTest, VectorDataName )
{
  EXPECT_STREQ("archimedesIBForce", qPrintable(log.vectorDataName));
}

TEST_F( LogDataReaderTest, ScalarDataName )
{
  EXPECT_STREQ("", qPrintable(log.scalarDataName));
}

