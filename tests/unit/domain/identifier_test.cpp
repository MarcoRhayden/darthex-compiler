#include <catch2/catch_test_macros.hpp>

#include "darthex/domain/identifier.hpp"

using darthex::domain::is_ascii_digit;
using darthex::domain::is_ascii_letter;
using darthex::domain::is_identifier_part;
using darthex::domain::is_identifier_start;

TEST_CASE("is_ascii_letter") {
  CHECK(is_ascii_letter('a'));
  CHECK(is_ascii_letter('z'));
  CHECK(is_ascii_letter('A'));
  CHECK(is_ascii_letter('Z'));
  CHECK_FALSE(is_ascii_letter('0'));
  CHECK_FALSE(is_ascii_letter('_'));
  CHECK_FALSE(is_ascii_letter('@'));
}

TEST_CASE("is_ascii_digit") {
  CHECK(is_ascii_digit('0'));
  CHECK(is_ascii_digit('9'));
  CHECK_FALSE(is_ascii_digit('a'));
  CHECK_FALSE(is_ascii_digit('_'));
}

TEST_CASE("is_identifier_start accepts letters and underscore only") {
  CHECK(is_identifier_start('a'));
  CHECK(is_identifier_start('Z'));
  CHECK(is_identifier_start('_'));
  CHECK_FALSE(is_identifier_start('0'));
  CHECK_FALSE(is_identifier_start('@'));
  CHECK_FALSE(is_identifier_start(' '));
}

TEST_CASE("is_identifier_part accepts letters, digits and underscore") {
  CHECK(is_identifier_part('a'));
  CHECK(is_identifier_part('Z'));
  CHECK(is_identifier_part('_'));
  CHECK(is_identifier_part('0'));
  CHECK(is_identifier_part('9'));
  CHECK_FALSE(is_identifier_part('@'));
  CHECK_FALSE(is_identifier_part(' '));
}
