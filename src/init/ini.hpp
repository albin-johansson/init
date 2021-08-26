#ifndef INIT_INI_HPP
#define INIT_INI_HPP

#include <filesystem>   // path
#include <fstream>      // ifstream, ofstream
#include <functional>   // less
#include <istream>      // basic_istream
#include <map>          // map
#include <ostream>      // basic_ostream
#include <stdexcept>    // out_of_range
#include <string>       // basic_string, char_traits
#include <string_view>  // basic_string_view
#include <utility>      // move

#include "common.hpp"
#include "ini_parser.hpp"
#include "ini_section.hpp"
#include "ini_value.hpp"

namespace init {

template <typename Char>
class basic_ini final
{
  friend class ini_parser<Char>;

 public:
  using char_type = Char;
  using string_type = std::basic_string<char_type>;
  using string_view_type = std::basic_string_view<char_type>;

  using parser_type = ini_parser<char_type>;
  using format_type = ini_format<char_type>;
  using section_type = basic_ini_section<char_type>;

  using storage_type = std::map<string_type, section_type, std::less<>>;
  using iterator = typename storage_type::iterator;
  using const_iterator = typename storage_type::const_iterator;

  using istream_type = std::basic_istream<char_type, std::char_traits<char_type>>;
  using ostream_type = std::basic_ostream<char_type, std::char_traits<char_type>>;
  using size_type = usize;

  explicit basic_ini(const format_type format = format_type{}) : m_format{format}
  {}

  basic_ini(const basic_ini&) = default;
  basic_ini(basic_ini&&) noexcept = default;

  basic_ini& operator=(const basic_ini&) = default;
  basic_ini& operator=(basic_ini&&) noexcept = default;

  void read(istream_type& stream)
  {
    parser_type parser;
    parser.parse_to(this, stream);
  }

  void dump(ostream_type& stream) const
  {
    for (const auto& [name, section] : m_sections)
    {
      stream << m_format.section_start << name << m_format.section_end << '\n';
      section.dump(stream, m_format);
    }
  }

  auto emplace_or_replace(string_type section) -> section_type&
  {
    const auto [it, inserted] =
        m_sections.insert_or_assign(std::move(section), section_type{});
    return it->second;
  }

  auto get_or_emplace(const string_view_type section) -> section_type&
  {
    if (const auto it = m_sections.find(section); it != m_sections.end())
    {
      return it->second;
    }
    else
    {
      auto [iterator, inserted] = m_sections.try_emplace(string_type{section});
      return iterator->second;
    }
  }

  /// \copydoc get_or_emplace()
  auto operator[](const string_view_type section) -> section_type&
  {
    return get_or_emplace(section);
  }

  [[nodiscard]] auto at(const string_view_type section) -> section_type&
  {
    if (const auto it = m_sections.find(section); it != m_sections.end())
    {
      return it->second;
    }
    else
    {
      throw std::out_of_range{"basic_ini::at(): section does not exist!"};
    }
  }

  [[nodiscard]] auto at(const string_view_type section) const -> const section_type&
  {
    if (const auto it = m_sections.find(section); it != m_sections.end())
    {
      return it->second;
    }
    else
    {
      throw std::out_of_range{"basic_ini::at(): section does not exist!"};
    }
  }

  auto erase(const string_view_type section) -> bool
  {
    if (const auto it = m_sections.find(section); it != m_sections.end())
    {
      m_sections.erase(it);
      return true;
    }
    else
    {
      return false;
    }
  }

  [[nodiscard]] auto contains(const string_view_type section) const -> bool
  {
    return m_sections.find(section) != m_sections.end();
  }

  [[nodiscard]] auto size() const noexcept -> size_type
  {
    return m_sections.size();
  }

  [[nodiscard]] auto empty() const noexcept -> bool
  {
    return m_sections.empty();
  }

  [[nodiscard]] auto begin() noexcept -> iterator
  {
    return m_sections.begin();
  }

  [[nodiscard]] auto begin() const noexcept -> const_iterator
  {
    return m_sections.begin();
  }

  [[nodiscard]] auto end() noexcept -> iterator
  {
    return m_sections.end();
  }

  [[nodiscard]] auto end() const noexcept -> const_iterator
  {
    return m_sections.end();
  }

 private:
  storage_type m_sections;
  format_type m_format;
};

using ini = basic_ini<char>;
using wini = basic_ini<wchar_t>;

template <typename Char = char>
[[nodiscard]] auto read_ini(const std::filesystem::path& path) -> basic_ini<Char>
{
  std::ifstream stream{path};

  basic_ini<Char> ini;
  ini.read(stream);

  return ini;
}

template <typename Char>
void write_ini(const basic_ini<Char>& ini, const std::filesystem::path& path)
{
  std::ofstream stream{path};
  ini.dump(stream);
}

}  // namespace init

#endif  // INIT_INI_HPP
