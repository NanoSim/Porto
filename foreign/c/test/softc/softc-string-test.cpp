#include <gtest/gtest.h>
#include <softc/softc-string.h>
#include <softc/softc-string-list.h>


TEST(StringTest, create)
{
  softc_string_s text = softc_string_create("The quick brown fox jumps over the lazy dog");
  const char *buff = from_softc_string(text);
  ASSERT_STREQ("The quick brown fox jumps over the lazy dog", buff);
  softc_string_destroy(text);
}

TEST(StringTest, assign)
{
  softc_string_s text = softc_string_create("abc");
  softc_string_assign(text, "A new string value");
  const char *buff = from_softc_string(text);
  ASSERT_STREQ("A new string value", buff);
  softc_string_destroy(text);
}

TEST(StringTest, compare)
{
  softc_string_s text = softc_string_create("Always rise "
					    "to an early meal, "
					    "but eat your fill before a feast.");
  ASSERT_EQ(0, softc_string_compare(text, "Always rise "
				    "to an early meal, "
				    "but eat your fill before a feast."));
  ASSERT_NE(0, softc_string_compare(text, "Another string"));
  softc_string_destroy(text);
}

TEST(StringTest, isequal)
{
  softc_string_s text1 = softc_string_create("If you're hungry "
					     "you have no time "
					     "to talk at the table.");
  softc_string_s text2 = softc_string_create("If you're hungry "
					     "you have no time "
					     "to talk at the table.");
  ASSERT_TRUE(softc_string_isequal(text1, text2));
  softc_string_assign(text2, "This string is changed...");
  ASSERT_FALSE(softc_string_isequal(text1, text2));
  softc_string_destroy(text1);
  softc_string_destroy(text2);
}
