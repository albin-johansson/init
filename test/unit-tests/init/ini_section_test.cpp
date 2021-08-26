#include "ini_section.hpp"

#include <gtest/gtest.h>

#include <algorithm>  // for_each
#include <iostream>   // cout
#include <string>     // to_string

TEST(IniSection, Defaults)
{
  const init::ini_section section;
  ASSERT_EQ(0, section.size());
  ASSERT_TRUE(section.empty());
}

TEST(IniSection, Dump)
{
  init::wini_section section;

  section[L"foo"] = L"bar";
  section[L"boo"] = 42;
  section[L"doo"] = 12.3;
  section[L"zoo"] = true;
  section[L"noo"] = 27u;

  std::cout << "Section with standard format: \n";
  section.dump(std::wcout, init::wini_section::format_type{});
}

TEST(IniSection, Erase)
{
  init::ini_section section;
  ASSERT_FALSE(section.erase("foo"));

  section["foo"] = "abc";
  section["bar"] = 42;
  ASSERT_EQ(2, section.size());
  ASSERT_TRUE(section.contains("foo"));
  ASSERT_TRUE(section.contains("bar"));

  ASSERT_TRUE(section.erase("foo"));
  ASSERT_FALSE(section.erase("foo"));
  ASSERT_FALSE(section.contains("foo"));
  ASSERT_EQ(1, section.size());

  ASSERT_FALSE(section.erase("Bar"));
  ASSERT_TRUE(section.erase("bar"));
  ASSERT_TRUE(section.empty());

  const std::string str = "bar";
  ASSERT_FALSE(section.erase(str));
}

TEST(IniSection, GetOrEmplace)
{
  init::ini_section section;

  auto& a = section.get_or_emplace("foo");
  ASSERT_EQ(1, section.size());
  ASSERT_TRUE(section.contains("foo"));

  auto& b = section.get_or_emplace("foo");
  ASSERT_EQ(1, section.size());
  ASSERT_EQ(&a, &b);

  section.get_or_emplace("bar") = 42u;
  ASSERT_EQ(2, section.size());
  ASSERT_EQ(42u, section.at("bar"));
}

TEST(IniSection, SubscriptOperator)
{
  init::ini_section section;

  auto& a = section["foo"];
  ASSERT_EQ(1, section.size());
  ASSERT_TRUE(section.contains("foo"));

  auto& b = section["foo"];
  ASSERT_EQ(1, section.size());
  ASSERT_EQ(&a, &b);

  section["bar"] = "hello";
  ASSERT_EQ(2, section.size());
  ASSERT_EQ("hello", section.at("bar"));
}

TEST(IniSection, At)
{
  init::ini_section section;
  const auto& ref = section;

  ASSERT_THROW(section.at("foo"), std::out_of_range);
  ASSERT_THROW(ref.at("foo"), std::out_of_range);

  section["foo"] = "bar";
  ASSERT_EQ("bar", section.at("foo"));
  ASSERT_EQ("bar", ref.at("foo"));
}

TEST(IniSection, Contains)
{
  init::ini_section section;
  ASSERT_FALSE(section.contains("foo"));

  section["foo"] = 123;
  ASSERT_TRUE(section.contains("foo"));

  section.erase("foo");
  ASSERT_FALSE(section.contains("foo"));
}

TEST(IniSection, Iteration)
{
  init::ini_section section;

  section["0"] = 0;
  section["1"] = 1;
  section["2"] = 2;

  int index = 0;
  for (const auto& [name, value] : section)
  {
    ASSERT_EQ(index, section.at(std::to_string(index)));
    ++index;
  }

  using pair_type = std::pair<const std::string, init::ini_value>;
  std::ranges::for_each(section, [](pair_type& elem) {
    auto& value = elem.second;
    value = value.as<int>() + 1;
  });

  index = 0;
  for (const auto& [name, value] : section)
  {
    ASSERT_EQ(index + 1, section.at(std::to_string(index)));
    ++index;
  }
}
