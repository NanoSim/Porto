#include <gtest/gtest.h>
#include <softc/softc-string.h>

TEST(SoftC_StringTest, create)
{
  softc_string_s text = softc_string_create("The quick brown fox jumps over the lazy dog");
  const char *buff = from_softc_string(text);
  ASSERT_STREQ(buff, "The quick brown fox jumps over the lazy dog");
  softc_string_destroy(text);
}
