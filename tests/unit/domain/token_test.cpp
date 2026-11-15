#include <catch2/catch_test_macros.hpp>

#include "darthex/domain/token.hpp"

using darthex::domain::SourceLocation;
using darthex::domain::Token;
using darthex::domain::TokenKind;

TEST_CASE("Token default kind is end_of_file") {
  Token t;
  CHECK(t.kind == TokenKind::end_of_file);
  CHECK(t.lexeme.empty());
}

TEST_CASE("Token equality") {
  Token a{TokenKind::identifier, "foo", SourceLocation{1, 1, 0}};
  Token b{TokenKind::identifier, "foo", SourceLocation{1, 1, 0}};
  Token c{TokenKind::identifier, "bar", SourceLocation{1, 1, 0}};
  Token d{TokenKind::kw_if,      "if",  SourceLocation{1, 1, 0}};

  CHECK(a == b);
  CHECK(a != c);
  CHECK(a != d);
}
