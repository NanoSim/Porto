#include <gtest/gtest.h>
#include <QH5>

using soft::hdf5::QH5;

class HDF5Test : public ::testing::Test {
protected:
  static void SetUpTestCase() {
  }
  
  static void TearDownTestCase() {
  }
  
  virtual void SetUp() {
  }
  
  virtual void TearDown() {
  }  
};

TEST_F (HDF5Test, create)
{
  QH5 *h5 = new QH5();
  ASSERT_TRUE(h5 != NULL);

  bool createOk = h5->create("myfile.h5");
  ASSERT_TRUE(createOk);
}
