#include "ini_value.hpp"

#include <gtest/gtest.h>

#include <sstream>  // ostringstream

TEST(IniValue, Defaults)
{
  const init::ini_value value;
  ASSERT_TRUE(value.is_string());
}

TEST(IniValue, DirectInitialization)
{
  const init::ini_value str{"foo"};
  ASSERT_TRUE(str.is_string());
  ASSERT_EQ("foo", str);
}

TEST(IniValue, StringValue)
{
  const init::ini_value value = "bar";
  ASSERT_TRUE(value.is_string());
  ASSERT_EQ("bar", value);
}

TEST(IniValue, WideStringValue)
{
  const init::wini_value value = L"abc";
  ASSERT_TRUE(value.is_string());
  ASSERT_EQ(L"abc", value);
}

TEST(IniValue, SignedPositiveIntegerValue)
{
  const init::ini_value value = 123'456;
  ASSERT_TRUE(value.is_int());
  ASSERT_FALSE(value.is_uint());
  ASSERT_EQ(123'456, value);
}

TEST(IniValue, SignedNegativeIntegerValue)
{
  const init::wini_value value = -123;
  ASSERT_TRUE(value.is_int());
  ASSERT_FALSE(value.is_uint());
  ASSERT_EQ(-123, value);
}

TEST(IniValue, UnsignedIntegerValue)
{
  const init::ini_value value = 1'563'329u;
  ASSERT_TRUE(value.is_uint());
  ASSERT_FALSE(value.is_int());
  ASSERT_EQ(1'563'329u, value);
}

TEST(IniValue, FloatValue)
{
  const init::ini_value value = 45.3;
  ASSERT_TRUE(value.is_float());
  ASSERT_EQ(45.3, value);
}

TEST(IniValue, BooleanValue)
{
  const init::ini_value a{true};
  const init::ini_value b{false};

  ASSERT_TRUE(a.is_bool());
  ASSERT_TRUE(b.is_bool());

  ASSERT_EQ(true, a);
  ASSERT_EQ(false, b);
}

TEST(IniValue, DumpString)
{
  const init::wini_value value{L"foo"};

  std::wostringstream stream;
  value.dump(stream);

  ASSERT_EQ(L"foo", stream.str());
}

TEST(IniValue, DumpSignedInteger)
{
  {
    const init::ini_value positive{123};

    std::ostringstream stream;
    positive.dump(stream);

    ASSERT_EQ("123", stream.str());
  }

  {
    const init::ini_value negative{-42};

    std::ostringstream stream;
    negative.dump(stream);

    ASSERT_EQ("-42", stream.str());
  }
}

TEST(IniValue, DumpUnsignedInteger)
{
  const init::ini_value value{1'337u};

  std::ostringstream stream;
  value.dump(stream);

  ASSERT_EQ("1337u", stream.str());
}

TEST(IniValue, DumpFloat)
{
  const init::ini_value value{56.4};

  std::ostringstream stream;
  value.dump(stream);

  ASSERT_EQ("56.400000", stream.str());
}

TEST(IniValue, DumpBoolean)
{
  {
    const init::ini_value value{true};

    std::ostringstream stream;
    value.dump(stream);

    ASSERT_EQ("true", stream.str());
  }

  {
    const init::ini_value value{false};

    std::ostringstream stream;
    value.dump(stream);

    ASSERT_EQ("false", stream.str());
  }
}

TEST(IniValue, GetToBool)
{
  const init::ini_value value{true};

  int i{};
  ASSERT_THROW(value.get_to(i), std::bad_variant_access);

  bool b{};
  value.get_to(b);

  ASSERT_TRUE(b);
}

TEST(IniValue, GetToInt8)
{
  const init::ini_value value = 105;

  bool b{};
  ASSERT_THROW(value.get_to(b), std::bad_variant_access);

  init::int8 i{};
  value.get_to(i);

  ASSERT_EQ(105, i);
}

TEST(IniValue, GetToInt16)
{
  const init::ini_value value = 1'234;

  std::string s;
  ASSERT_THROW(value.get_to(s), std::bad_variant_access);

  init::int16 i{};
  value.get_to(i);

  ASSERT_EQ(1'234, i);
}

TEST(IniValue, GetToInt32)
{
  const init::ini_value value = -5'721;

  init::uint8 u{};
  ASSERT_THROW(value.get_to(u), std::bad_variant_access);

  init::int32 i{};
  value.get_to(i);

  ASSERT_EQ(-5'721, i);
}

TEST(IniValue, GetToInt64)
{
  const init::ini_value value = 284'482;

  float f{};
  ASSERT_THROW(value.get_to(f), std::bad_variant_access);

  init::int64 i{};
  value.get_to(i);

  ASSERT_EQ(284'482, i);
}

TEST(IniValue, GetToUint8)
{
  const init::ini_value value = 12u;

  double d{};
  ASSERT_THROW(value.get_to(d), std::bad_variant_access);

  init::uint8 u{};
  value.get_to(u);

  ASSERT_EQ(12u, u);
}

TEST(IniValue, GetToUint16)
{
  const init::ini_value value = 468u;

  bool b{};
  ASSERT_THROW(value.get_to(b), std::bad_variant_access);

  init::uint16 u{};
  value.get_to(u);

  ASSERT_EQ(468u, u);
}

TEST(IniValue, GetToUint32)
{
  const init::ini_value value = 1'395u;

  init::int32 i{};
  ASSERT_THROW(value.get_to(i), std::bad_variant_access);

  init::uint32 u{};
  value.get_to(u);

  ASSERT_EQ(1'395u, u);
}

TEST(IniValue, GetToUint64)
{
  const init::ini_value value = 92'564u;

  std::string s;
  ASSERT_THROW(value.get_to(s), std::bad_variant_access);

  init::uint64 u{};
  value.get_to(u);

  ASSERT_EQ(92'564u, u);
}

TEST(IniValue, GetToFloat)
{
  const init::ini_value value = 54.3f;

  int i{};
  ASSERT_THROW(value.get_to(i), std::bad_variant_access);

  float f{};
  value.get_to(f);

  ASSERT_EQ(54.3f, f);
}

TEST(IniValue, GetToDouble)
{
  const init::ini_value value = 164.8;

  int i{};
  ASSERT_THROW(value.get_to(i), std::bad_variant_access);

  double d{};
  value.get_to(d);

  ASSERT_EQ(164.8, d);
}

TEST(IniValue, TryAs)
{
  init::ini_value value;

  value = "foo";
  ASSERT_TRUE(value.try_as_string());
  ASSERT_FALSE(value.try_as_int());
  ASSERT_FALSE(value.try_as_uint());
  ASSERT_FALSE(value.try_as_float());
  ASSERT_FALSE(value.try_as_bool());

  value = 123;
  ASSERT_FALSE(value.try_as_string());
  ASSERT_TRUE(value.try_as_int());
  ASSERT_FALSE(value.try_as_uint());
  ASSERT_FALSE(value.try_as_float());
  ASSERT_FALSE(value.try_as_bool());

  value = 42u;
  ASSERT_FALSE(value.try_as_string());
  ASSERT_FALSE(value.try_as_int());
  ASSERT_TRUE(value.try_as_uint());
  ASSERT_FALSE(value.try_as_float());
  ASSERT_FALSE(value.try_as_bool());

  value = 12.3;
  ASSERT_FALSE(value.try_as_string());
  ASSERT_FALSE(value.try_as_int());
  ASSERT_FALSE(value.try_as_uint());
  ASSERT_TRUE(value.try_as_float());
  ASSERT_FALSE(value.try_as_bool());

  value = true;
  ASSERT_FALSE(value.try_as_string());
  ASSERT_FALSE(value.try_as_int());
  ASSERT_FALSE(value.try_as_uint());
  ASSERT_FALSE(value.try_as_float());
  ASSERT_TRUE(value.try_as_bool());
}

TEST(IniValue, TypeIndicators)
{
  init::ini_value value = "foo";
  ASSERT_TRUE(value.is_string());
  ASSERT_FALSE(value.is_int());
  ASSERT_FALSE(value.is_uint());
  ASSERT_FALSE(value.is_float());
  ASSERT_FALSE(value.is_bool());

  value = 27;
  ASSERT_FALSE(value.is_string());
  ASSERT_TRUE(value.is_int());
  ASSERT_FALSE(value.is_uint());
  ASSERT_FALSE(value.is_float());
  ASSERT_FALSE(value.is_bool());

  value = 452u;
  ASSERT_FALSE(value.is_string());
  ASSERT_FALSE(value.is_int());
  ASSERT_TRUE(value.is_uint());
  ASSERT_FALSE(value.is_float());
  ASSERT_FALSE(value.is_bool());

  value = 542.8f;
  ASSERT_FALSE(value.is_string());
  ASSERT_FALSE(value.is_int());
  ASSERT_FALSE(value.is_uint());
  ASSERT_TRUE(value.is_float());
  ASSERT_FALSE(value.is_bool());

  value = false;
  ASSERT_FALSE(value.is_string());
  ASSERT_FALSE(value.is_int());
  ASSERT_FALSE(value.is_uint());
  ASSERT_FALSE(value.is_float());
  ASSERT_TRUE(value.is_bool());
}

TEST(IniValue, Comparison)
{
  ASSERT_EQ(init::ini_value{}, init::ini_value{});

  ASSERT_EQ(init::ini_value{42}, init::ini_value{42});
  ASSERT_NE(init::ini_value{42}, init::ini_value{42u});
  ASSERT_NE(init::ini_value{42}, init::ini_value{123});

  ASSERT_NE(init::ini_value{123u}, init::ini_value{"foo"});
  ASSERT_EQ(init::ini_value{12.3}, init::ini_value{12.3});

  ASSERT_EQ(init::ini_value{true}, init::ini_value{true});
  ASSERT_EQ(init::ini_value{false}, init::ini_value{false});
  ASSERT_NE(init::ini_value{true}, init::ini_value{false});
}
