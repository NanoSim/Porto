#include <gtest/gtest.h>
#include <softc/softc-string-list.h>
#include <softc/softc-string.h>

class StringListTest : public ::testing::Test {
protected:
  static void SetUpTestCase() {}
  static void TearDownTestCase() {}
};

TEST_F(StringListTest, create)
{
  auto lst = softc_string_list_create();
  ASSERT_TRUE(lst != nullptr);
  ASSERT_EQ(softc_string_list_count(lst), 0);
  softc_string_list_free(lst);
}

TEST_F(StringListTest, append)
{
  auto lst = softc_string_list_create();
  auto str1 = softc_string_create("Hello");
  auto str2 = softc_string_create("Kitty");
  ASSERT_TRUE(lst != nullptr);
  ASSERT_TRUE(str1 != nullptr);
  ASSERT_TRUE(str2 != nullptr);

  softc_string_list_append(lst, str1);
  softc_string_list_append(lst, str2);
  ASSERT_EQ(softc_string_list_count(lst), 2);
}

TEST_F(StringListTest, at)
{
  auto lst = softc_string_list_create();
  auto str1 = softc_string_create("a");
  auto str2 = softc_string_create("b");
  auto str3 = softc_string_create("c");
  ASSERT_TRUE(lst != nullptr);
  ASSERT_TRUE(str1 != nullptr);
  ASSERT_TRUE(str2 != nullptr);
  ASSERT_TRUE(str3 != nullptr);

  softc_string_list_append(lst, str1);
  softc_string_list_append(lst, str2);
  softc_string_list_append(lst, str3);

  auto s1 = softc_string_at(lst, 0);
  auto s2 = softc_string_at(lst, 1);
  auto s3 = softc_string_at(lst, 2);
  ASSERT_TRUE(softc_string_isequal(str1, s1));
  ASSERT_TRUE(softc_string_isequal(str2, s2));
  ASSERT_TRUE(softc_string_isequal(str3, s3));
  ASSERT_EQ(softc_string_list_count(lst), 3);
}
