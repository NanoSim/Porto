#include <gtest/gtest.h>
#include <cstdlib>
#include <softc/softc-utils.h>
#include <softc/softc-entity.h>

#include "foo.h"

class EntityTest : public ::testing::Test {
protected:
  static void SetUpTestCase() {}
  static void TearDownTestCase() {}    
};

typedef char* soft_string;
typedef double* soft_double_array;

TEST_F(EntityTest, construct)
{
  double a[] = {0.0, 1.0, 2.0, 3.0, 4.0};
  size_t len = sizeof(a)/sizeof(a[0]);
  ASSERT_EQ(5, len);
  
  foo_s *foo = foo_create(NULL, 10);
  foo_property_set_n(foo, 42);
  foo_property_set_str(foo, "Dette er en test");
  foo_property_set_array(foo, a, len);

  ASSERT_EQ(len, foo_dimension_get_i(foo));

  int ret_n;
  foo_property_get_n(foo, &ret_n);
  ASSERT_EQ(ret_n, 42);

  soft_string ret_str = NULL;
  foo_property_get_str(foo, &ret_str);
  ASSERT_STREQ(ret_str, "Dette er en test");
  free (ret_str);
  soft_double_array arr = NULL;
  size_t arr_len;
  foo_property_get_array(foo, &arr, &arr_len);
  for (unsigned int i = 0; i < arr_len; ++i) {
    ASSERT_EQ(arr[i], i*1.0);
  }
  free(arr);
  foo_free(foo);
}
