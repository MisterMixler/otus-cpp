#pragma once

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <list>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace ip_print {

namespace detail {

template <class...>
inline constexpr bool dependent_false_v = false;

template <class T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

template <class T, template <class...> class Template>
struct is_specialization_of : std::false_type {};

template <template <class...> class Template, class... Args>
struct is_specialization_of<Template<Args...>, Template> : std::true_type {};

template <class T>
inline constexpr bool is_std_vector_v =
    is_specialization_of<remove_cvref_t<T>, std::vector>::value;

template <class T>
inline constexpr bool is_std_list_v =
    is_specialization_of<remove_cvref_t<T>, std::list>::value;

template <class T>
inline constexpr bool is_supported_container_v = is_std_vector_v<T> || is_std_list_v<T>;

template <class T>
void print_joined(const T& value, std::ostream& os, char /*sep*/) {
  os << value;
}

template <class It>
void print_range(It first, It last, std::ostream& os, char sep) {
  if (first == last) {
    return;
  }
  os << *first;
  ++first;
  for (; first != last; ++first) {
    os << sep << *first;
  }
}

template <class... Ts>
struct all_same : std::true_type {};

template <class T0, class... Ts>
struct all_same<T0, Ts...>
    : std::bool_constant<(std::is_same_v<T0, Ts> && ...)> {};

template <class Tuple, std::size_t... Is>
void print_tuple_impl(const Tuple& t, std::index_sequence<Is...>, std::ostream& os, char sep) {
  bool first = true;
  (void)std::initializer_list<int>{
      (first ? (first = false, os << std::get<Is>(t))
             : (os << sep << std::get<Is>(t)),
       0)...};
}

}  // namespace detail

/// Print conditional IP address from an integral value.
///
/// Prints all bytes of the value as unsigned integers, from the most significant
/// byte to the least significant, separated by '.'.
template <class T,
          std::enable_if_t<std::is_integral_v<detail::remove_cvref_t<T>>, int> = 0>
void print_ip(T value, std::ostream& os = std::cout) {
  using U = std::make_unsigned_t<detail::remove_cvref_t<T>>;
  U u = static_cast<U>(value);

  constexpr std::size_t n = sizeof(U);
  for (std::size_t i = 0; i < n; ++i) {
    const std::size_t shift = (n - 1 - i) * 8;
    const auto byte = static_cast<unsigned int>((u >> shift) & static_cast<U>(0xFF));
    if (i != 0) {
      os << '.';
    }
    os << byte;
  }
  os << '\n';
}

/// Print conditional IP address from a string.
inline void print_ip(std::string_view value, std::ostream& os = std::cout) {
  os << value << '\n';
}

/// Print conditional IP address from std::vector or std::list.
template <class T,
          std::enable_if_t<detail::is_supported_container_v<T>, int> = 0>
void print_ip(const T& container, std::ostream& os = std::cout) {
  detail::print_range(container.begin(), container.end(), os, '.');
  os << '\n';
}

/// Print conditional IP address from std::tuple where all types are the same.
template <class... Ts>
void print_ip(const std::tuple<Ts...>& t, std::ostream& os = std::cout) {
  static_assert(detail::all_same<Ts...>::value,
                "print_ip(tuple): all tuple element types must be the same");
  detail::print_tuple_impl(t, std::index_sequence_for<Ts...>{}, os, '.');
  os << '\n';
}

}  // namespace ip_print

