#include <gtest/gtest.h>
#include <cstdlib>
#include <softc/softc-types.h>

class TypesTest : public ::testing::Test {
protected:
  static void SetUpTestCase() {}
  static void TearDownTestCase() {}    
};


TEST_F(TypesTest, double_array)
{
  softc_double_array_s *da = softc_double_array_create();
  ASSERT_TRUE(softc_double_array_empty(da));
  
  size_t da_size = softc_double_array_size(da);
  ASSERT_EQ(da_size, 0);

  softc_double_array_resize(da, 10);
  da_size = softc_double_array_size(da);
  ASSERT_FALSE(softc_double_array_empty(da));
  ASSERT_EQ(da_size, 10);

  softc_double_array_free(&da);
  ASSERT_TRUE(da == NULL);
}

typedef softc_double_array_s * std_double_array;
TEST_F(TypesTest, double_array_value)
{  
  std_double_array da = softc_double_array_create();
  const double compare = 1.23;
  softc_double_array_resize_val(da, 10, compare);
  
  for (size_t i = 0; i < softc_double_array_size(da); ++i) {
    ASSERT_EQ(softc_double_array_at(da, i), compare);
  }
  softc_double_array_free(&da);
}

struct ftest {
  ftest():count(0),sum(0.0){}
  int count;
  double sum;
};

void callback(double v, void *data)
{
  ftest *f = (ftest *)data;
  f->count++;
  f->sum += v;
}

TEST_F(TypesTest, double_array_foreach)
{
  ftest *f = new ftest();  
  std_double_array da = softc_double_array_create();
  const double v = 1.23;
  softc_double_array_resize_val(da, 10, v);
  softc_double_array_foreach(da, callback, (void*)f);
  
  ASSERT_EQ(f->count, 10);
  ASSERT_DOUBLE_EQ(f->sum, v*10.0);

  softc_double_array_free(&da);
  delete f;
}
