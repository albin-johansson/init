#include "ini.hpp"

#include <gtest/gtest.h>

#include <fstream>  // ifstream

TEST(Ini, Defaults)
{
  const init::ini ini;
  ASSERT_TRUE(ini.empty());
  ASSERT_EQ(0, ini.size());
}

TEST(Ini, ReadIni)
{
  const auto ini = init::read_ini("resources/test.ini");

  ASSERT_FALSE(ini.empty());
  ASSERT_EQ(2, ini.size());

  {
    const auto& foo = ini.at("foo");
    ASSERT_EQ(5, foo.size());

    ASSERT_TRUE(foo.contains("str_1"));
    ASSERT_TRUE(foo.at("str_1").is_string());
    ASSERT_EQ("hello12.3", foo.at("str_1"));

    ASSERT_TRUE(foo.contains("str_2"));
    ASSERT_TRUE(foo.at("str_2").is_string());
    ASSERT_EQ("12a", foo.at("str_2"));

    ASSERT_TRUE(foo.contains("unsigned"));
    ASSERT_TRUE(foo.at("unsigned").is_uint());
    ASSERT_EQ(87u, foo.at("unsigned"));

    ASSERT_TRUE(foo.contains("positive"));
    ASSERT_TRUE(foo.at("positive").is_int());
    ASSERT_EQ(123, foo.at("positive"));

    ASSERT_TRUE(foo.contains("negative"));
    ASSERT_TRUE(foo.at("negative").is_int());
    ASSERT_EQ(-42, foo.at("negative"));
  }

  {
    const auto& bar = ini.at("bar");
    ASSERT_EQ(3, bar.size());

    ASSERT_TRUE(bar.contains("real"));
    ASSERT_TRUE(bar.at("real").is_float());
    ASSERT_EQ(32.1, bar.at("real"));

    ASSERT_TRUE(bar.contains("bool_t"));
    ASSERT_TRUE(bar.at("bool_t").is_bool());
    ASSERT_EQ(true, bar.at("bool_t"));

    ASSERT_TRUE(bar.contains("bool_f"));
    ASSERT_TRUE(bar.at("bool_f").is_bool());
    ASSERT_EQ(false, bar.at("bool_f"));
  }
}

TEST(Ini, WriteRead)
{
  {
    init::ini ini;
    ini["foo"]["positive"] = 9'394;
    ini["foo"]["negative"] = -834;
    ini["foo"]["unsigned"] = 734u;
    ini["foo"]["str"] = "foobar";

    ini["bar"]["real_1"] = 1.0;
    ini["bar"]["real_2"] = 87.3;
    ini["bar"]["real_3"] = 0.0;
    ini["bar"]["bool_f"] = false;
    ini["bar"]["bool_t"] = true;

    init::write_ini(ini, "write_read.ini");
  }

  {
    const auto ini = init::read_ini("write_read.ini");

    const auto& foo = ini.at("foo");
    const auto& bar = ini.at("bar");

    ASSERT_TRUE(foo.at("positive").is_int());
    ASSERT_EQ(9'394, foo.at("positive").as<int>());

    ASSERT_TRUE(foo.at("negative").is_int());
    ASSERT_EQ(-834, foo.at("negative").as<int>());

    ASSERT_TRUE(foo.at("unsigned").is_uint());
    ASSERT_EQ(734u, foo.at("unsigned").as<unsigned>());

    ASSERT_TRUE(foo.at("str").is_string());
    ASSERT_EQ("foobar", foo.at("str").as<std::string>());

    ASSERT_TRUE(bar.at("real_1").is_float());
    ASSERT_EQ(1.0f, bar.at("real_1").as<float>());

    ASSERT_TRUE(bar.at("real_2").is_float());
    ASSERT_EQ(87.3f, bar.at("real_2").as<float>());

    ASSERT_TRUE(bar.at("real_3").is_float());
    ASSERT_EQ(0.0f, bar.at("real_3").as<float>());

    ASSERT_TRUE(bar.at("bool_f").is_bool());
    ASSERT_EQ(false, bar.at("bool_f").as<bool>());

    ASSERT_TRUE(bar.at("bool_t").is_bool());
    ASSERT_EQ(true, bar.at("bool_t").as<bool>());
  }
}

TEST(Ini, EmplaceOrReplace)
{
  init::ini ini;

  {
    auto& foo = ini.emplace_or_replace("foo");
    foo["abc"] = 42;

    ASSERT_TRUE(ini.contains("foo"));
    ASSERT_EQ(1, ini.size());
  }

  {
    auto& foo = ini.emplace_or_replace("foo");
    ASSERT_EQ(1, ini.size());

    // Ensure that the section was replaced
    ASSERT_FALSE(ini.contains("abc"));
  }
}

TEST(Ini, GetOrEmplace)
{
  init::ini ini;

  {
    auto& foo = ini.get_or_emplace("foo");
    ASSERT_TRUE(ini.contains("foo"));
    ASSERT_EQ(1, ini.size());

    foo["abc"] = "123";
  }

  {
    auto& foo = ini.get_or_emplace("foo");
    ASSERT_TRUE(foo.contains("abc"));

    ASSERT_TRUE(ini.contains("foo"));
    ASSERT_EQ(1, ini.size());
  }

  ini.get_or_emplace("bar");
  ASSERT_TRUE(ini.contains("bar"));
  ASSERT_EQ(2, ini.size());
}

TEST(Ini, SubscriptOperator)
{
  init::ini ini;

  {
    auto& foo = ini["foo"];
    ASSERT_TRUE(ini.contains("foo"));
    ASSERT_EQ(1, ini.size());

    foo["abc"] = "123";
  }

  {
    auto& foo = ini["foo"];
    ASSERT_TRUE(foo.contains("abc"));

    ASSERT_TRUE(ini.contains("foo"));
    ASSERT_EQ(1, ini.size());
  }

  ini["bar"];
  ASSERT_TRUE(ini.contains("bar"));
  ASSERT_EQ(2, ini.size());
}

TEST(Ini, At)
{
  init::ini ini;
  const auto& ref = ini;

  ASSERT_THROW(ini.at("foo"), std::out_of_range);
  ASSERT_THROW(ref.at("foo"), std::out_of_range);

  ini["foo"];
  ini["bar"];

  ASSERT_NO_THROW(ini.at("foo"));
  ASSERT_NO_THROW(ini.at("bar"));

  ASSERT_NO_THROW(ref.at("foo"));
  ASSERT_NO_THROW(ref.at("bar"));
}

TEST(Ini, Erase)
{
  init::ini ini;
  ASSERT_FALSE(ini.erase("foo"));

  ini["foo"];
  ASSERT_TRUE(ini.contains("foo"));
  ASSERT_EQ(1, ini.size());

  ASSERT_TRUE(ini.erase("foo"));
  ASSERT_FALSE(ini.contains("foo"));
  ASSERT_EQ(0, ini.size());
  ASSERT_TRUE(ini.empty());

  ASSERT_FALSE(ini.erase("foo"));
}

TEST(Ini, Contains)
{
  init::ini ini;
  ASSERT_FALSE(ini.contains("abc"));

  ini["foo"];
  ASSERT_TRUE(ini.contains("foo"));

  ini.erase("foo");
  ASSERT_FALSE(ini.contains("foo"));
}
