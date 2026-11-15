#pragma once

namespace darthex::domain {

constexpr bool is_ascii(char c) {
  return static_cast<unsigned char>(c) <= 127;
}

constexpr bool is_utf8_continuation(char c) {
  return (static_cast<unsigned char>(c) & 0xC0U) == 0x80U;
}

constexpr bool is_horizontal_space(char c) {
  return c == ' ' || c == '\t' || c == '\v' || c == '\f';
}

constexpr bool is_line_break(char c) {
  return c == '\n' || c == '\r';
}

}  // namespace darthex::domain
