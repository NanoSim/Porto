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
  EXPECT_EQ(log.processor, 1);
}

TEST_F( LogDataReaderTest, VectorDataName )
{
  EXPECT_STREQ("archimedesIBForce", qPrintable(log.vectorDataName));
}

TEST_F( LogDataReaderTest, ScalarDataName )
{
  EXPECT_STREQ("", qPrintable(log.scalarDataName));
}

TEST_F( LogDataReaderTest, DataSizes )
{
  auto indexSize = log.index.size();
  EXPECT_EQ( log.vectorData.size(), indexSize ); 
  EXPECT_EQ( log.time.size(),       indexSize );
  EXPECT_EQ( log.vectorData.size(), indexSize );
  EXPECT_EQ( log.position.size(),   indexSize );
}

TEST_F( LogDataReaderTest, Contents )
{
  EXPECT_EQ( log.time[0], 0.001 );
  EXPECT_EQ( log.time[1], 0.002 );
  EXPECT_EQ( log.time[2], 0.003 );
  EXPECT_EQ( log.time[3], 0.004 );

  EXPECT_TRUE( log.vectorData[0] == QVector<double>({0,0,0}) );
  EXPECT_TRUE( log.position[0] == QVector<double>({4,4,4}) );
  EXPECT_FALSE(log.position[0] == QVector<double>({3,3,3}) );

}
