#include <catch2/catch_test_macros.hpp>

#include "darthex/domain/char_class.hpp"

using darthex::domain::is_ascii;
using darthex::domain::is_horizontal_space;
using darthex::domain::is_line_break;
using darthex::domain::is_utf8_continuation;

TEST_CASE("is_ascii accepts 0–127 and rejects 128–255") {
  CHECK(is_ascii('\0'));
  CHECK(is_ascii('a'));
  CHECK(is_ascii('~'));
  CHECK(is_ascii('\x7f'));
  CHECK_FALSE(is_ascii('\x80'));
  CHECK_FALSE(is_ascii('\xff'));
}

TEST_CASE("is_utf8_continuation identifies 10xxxxxx bytes") {
  CHECK(is_utf8_continuation('\x80'));
  CHECK(is_utf8_continuation('\xbf'));
  CHECK_FALSE(is_utf8_continuation('\x7f'));
  CHECK_FALSE(is_utf8_continuation('\xc0'));
}

TEST_CASE("is_horizontal_space") {
  CHECK(is_horizontal_space(' '));
  CHECK(is_horizontal_space('\t'));
  CHECK(is_horizontal_space('\v'));
  CHECK(is_horizontal_space('\f'));
  CHECK_FALSE(is_horizontal_space('\n'));
  CHECK_FALSE(is_horizontal_space('a'));
}

TEST_CASE("is_line_break") {
  CHECK(is_line_break('\n'));
  CHECK(is_line_break('\r'));
  CHECK_FALSE(is_line_break(' '));
  CHECK_FALSE(is_line_break('a'));
}
