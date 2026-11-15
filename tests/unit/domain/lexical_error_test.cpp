#include <catch2/catch_test_macros.hpp>

#include "darthex/domain/lexical_error.hpp"

using darthex::domain::LexicalError;
using darthex::domain::LexicalErrorKind;
using darthex::domain::SourceLocation;
using darthex::domain::lexical_error_kind_name;

TEST_CASE("LexicalError default kind is invalid_character") {
  LexicalError e;
  CHECK(e.kind == LexicalErrorKind::invalid_character);
  CHECK(e.lexeme.empty());
}

TEST_CASE("LexicalError equality") {
  LexicalError a{LexicalErrorKind::unterminated_string, "\"hello", SourceLocation{3, 1, 20}};
  LexicalError b{LexicalErrorKind::unterminated_string, "\"hello", SourceLocation{3, 1, 20}};
  LexicalError c{LexicalErrorKind::invalid_character,   "@",       SourceLocation{1, 5,  4}};

  CHECK(a == b);
  CHECK(a != c);
}

TEST_CASE("lexical_error_kind_name returns non-empty string for every variant") {
  const LexicalErrorKind kinds[] = {
    LexicalErrorKind::invalid_character,
    LexicalErrorKind::identifier_starts_with_digit,
    LexicalErrorKind::unterminated_comment,
    LexicalErrorKind::unterminated_char_const,
    LexicalErrorKind::invalid_char_const,
    LexicalErrorKind::unterminated_string,
  };

  for (auto k : kinds) {
    CHECK_FALSE(lexical_error_kind_name(k).empty());
  }
}
