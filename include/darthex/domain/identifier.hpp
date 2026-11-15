#pragma once

namespace darthex::domain {

constexpr bool is_ascii_letter(char c) {
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

constexpr bool is_ascii_digit(char c) {
  return c >= '0' && c <= '9';
}

constexpr bool is_identifier_start(char c) {
  return is_ascii_letter(c) || c == '_';
}

constexpr bool is_identifier_part(char c) {
  return is_identifier_start(c) || is_ascii_digit(c);
}

}  // namespace darthex::domain
