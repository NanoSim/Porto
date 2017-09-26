#ifndef SOFTC_STRING_PRIVATE_H_CPP
#define SOFTC_STRING_PRIVATE_H_CPP

#include <string>
#include <utility>      // std::move

struct _softc_string_s
{
  _softc_string_s(){}
  explicit _softc_string_s(const char *str = nullptr)
    :str(str)
  {}
  explicit _softc_string_s(_softc_string_s &&other) noexcept
    :str(std::move(other.str))
  {}

  _softc_string_s& operator=(_softc_string_s &&other)
  {
    str = std::move(other.str);
    return *this;
  }

  bool operator==(_softc_string_s &&other)
  {
    return (str == std::move(other.str));
  }

  int compare(const char *buffer)
  {
    return str.compare(buffer);
  }

  std::string str;
};

#endif
