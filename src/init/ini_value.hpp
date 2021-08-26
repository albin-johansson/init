#ifndef INIT_INI_VALUE_HPP
#define INIT_INI_VALUE_HPP

#include <concepts>  // integral, floating_point, constructible_from, same_as, ...
#include <ostream>   // basic_ostream
#include <string>    // basic_string, to_string, char_traits
#include <utility>   // move
#include <variant>   // variant, get, get_if, holds_alternative

#include "common.hpp"

namespace init {

template <typename T, typename Char>
concept is_ini_value = std::constructible_from<std::basic_string<Char>, T> ||
    std::integral<T> || std::floating_point<T>;

template <typename Char>
class basic_ini_value final
{
 public:
  using char_type = Char;
  using int_type = int64;
  using uint_type = uint64;
  using float_type = double;
  using string_type = std::basic_string<char_type>;
  using value_type = std::variant<string_type, bool, int_type, uint_type, float_type>;
  using ostream_type = std::basic_ostream<char_type, std::char_traits<char_type>>;

  basic_ini_value() = default;

  basic_ini_value(const basic_ini_value&) = default;
  basic_ini_value(basic_ini_value&&) noexcept = default;

  basic_ini_value& operator=(const basic_ini_value&) = default;
  basic_ini_value& operator=(basic_ini_value&&) noexcept = default;

  template <is_ini_value<char_type> T>
  /*implicit*/ basic_ini_value(T value)  // NOLINT
  {
    assign(std::move(value));
  }

  template <is_ini_value<char_type> T>
  auto operator=(T value) -> basic_ini_value&
  {
    assign(std::move(value));
    return *this;
  }

  void dump(ostream_type& stream) const
  {
    if (const auto* str = try_as_string())
    {
      stream << *str;
    }
    else if (const auto* i = try_as_int())
    {
      stream << std::to_string(*i);
    }
    else if (const auto* u = try_as_uint())
    {
      stream << std::to_string(*u) << 'u';
    }
    else if (const auto* f = try_as_float())
    {
      stream << std::to_string(*f);
    }
    else if (const auto* b = try_as_bool())
    {
      stream << ((*b) ? "true" : "false");
    }
  }

  template <is_ini_value<char_type> T>
  [[nodiscard]] auto as() const -> T
  {
    if constexpr (std::same_as<T, bool>)
    {
      return std::get<bool>(m_value);
    }
    else if constexpr (std::signed_integral<T>)
    {
      return static_cast<T>(std::get<int_type>(m_value));
    }
    else if constexpr (std::unsigned_integral<T>)
    {
      return static_cast<T>(std::get<uint_type>(m_value));
    }
    else if constexpr (std::floating_point<T>)
    {
      return static_cast<T>(std::get<float_type>(m_value));
    }
    else
    {
      return std::get<string_type>(m_value);
    }
  }

  void get_to(string_type& value) const
  {
    value = std::get<string_type>(m_value);
  }

  void get_to(bool& value) const
  {
    value = std::get<bool>(m_value);
  }

  void get_to(int8& value) const
  {
    value = static_cast<int8>(std::get<int_type>(m_value));
  }

  void get_to(int16& value) const
  {
    value = static_cast<int16>(std::get<int_type>(m_value));
  }

  void get_to(int32& value) const
  {
    value = static_cast<int32>(std::get<int_type>(m_value));
  }

  void get_to(int64& value) const
  {
    value = static_cast<int64>(std::get<int_type>(m_value));
  }

  void get_to(uint8& value) const
  {
    value = static_cast<uint8>(std::get<uint_type>(m_value));
  }

  void get_to(uint16& value) const
  {
    value = static_cast<uint16>(std::get<uint_type>(m_value));
  }

  void get_to(uint32& value) const
  {
    value = static_cast<uint32>(std::get<uint_type>(m_value));
  }

  void get_to(uint64& value) const
  {
    value = static_cast<uint64>(std::get<uint_type>(m_value));
  }

  void get_to(float& value) const
  {
    value = static_cast<float>(std::get<float_type>(m_value));
  }

  void get_to(double& value) const
  {
    value = static_cast<double>(std::get<float_type>(m_value));
  }

  void get_to(long double& value) const
  {
    value = static_cast<long double>(std::get<float_type>(m_value));
  }

  [[nodiscard]] auto try_as_string() const noexcept -> const string_type*
  {
    return std::get_if<string_type>(&m_value);
  }

  [[nodiscard]] auto try_as_bool() const noexcept -> const bool*
  {
    return std::get_if<bool>(&m_value);
  }

  [[nodiscard]] auto try_as_int() const noexcept -> const int_type*
  {
    return std::get_if<int_type>(&m_value);
  }

  [[nodiscard]] auto try_as_uint() const noexcept -> const uint_type*
  {
    return std::get_if<uint_type>(&m_value);
  }

  [[nodiscard]] auto try_as_float() const noexcept -> const float_type*
  {
    return std::get_if<float_type>(&m_value);
  }

  [[nodiscard]] auto is_string() const noexcept -> bool
  {
    return std::holds_alternative<string_type>(m_value);
  }

  [[nodiscard]] auto is_bool() const noexcept -> bool
  {
    return std::holds_alternative<bool>(m_value);
  }

  [[nodiscard]] auto is_int() const noexcept -> bool
  {
    return std::holds_alternative<int_type>(m_value);
  }

  [[nodiscard]] auto is_uint() const noexcept -> bool
  {
    return std::holds_alternative<uint_type>(m_value);
  }

  [[nodiscard]] auto is_float() const noexcept -> bool
  {
    return std::holds_alternative<float_type>(m_value);
  }

  [[nodiscard]] bool operator==(const basic_ini_value&) const = default;

 private:
  value_type m_value;

  template <is_ini_value<char_type> T>
  void assign(T value)
  {
    if constexpr (std::same_as<T, bool>)
    {
      m_value.template emplace<bool>(std::move(value));
    }
    else if constexpr (std::signed_integral<T>)
    {
      m_value.template emplace<int_type>(std::move(value));
    }
    else if constexpr (std::unsigned_integral<T>)
    {
      m_value.template emplace<uint_type>(std::move(value));
    }
    else if constexpr (std::floating_point<T>)
    {
      m_value.template emplace<float_type>(std::move(value));
    }
    else
    {
      m_value.template emplace<string_type>(std::move(value));
    }
  }
};

using ini_value = basic_ini_value<char>;
using wini_value = basic_ini_value<wchar_t>;

template <typename Char>
auto operator<<(typename basic_ini_value<Char>::ostream_type& stream,
                const basic_ini_value<Char>& value) ->
    typename basic_ini_value<Char>::ostream_type&
{
  value.dump(stream);
  return stream;
}

}  // namespace init

#endif  // INIT_INI_VALUE_HPP
