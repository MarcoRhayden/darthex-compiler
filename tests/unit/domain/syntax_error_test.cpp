#include <catch2/catch_test_macros.hpp>

#include "darthex/domain/syntax_error.hpp"

using darthex::domain::SourceLocation;
using darthex::domain::SyntaxError;
using darthex::domain::Token;
using darthex::domain::TokenKind;

TEST_CASE("SyntaxError stores the unexpected token and expectation") {
  const SyntaxError error{
      Token{TokenKind::identifier, "value", SourceLocation{3, 5, 20}},
      "'begin'",
  };

  CHECK(error.unexpected.kind == TokenKind::identifier);
  CHECK(error.unexpected.lexeme == "value");
  CHECK(error.unexpected.location == SourceLocation{3, 5, 20});
  CHECK(error.expected == "'begin'");
}

TEST_CASE("SyntaxError compares by value") {
  const SyntaxError error{
      Token{TokenKind::end_of_file, "", SourceLocation{8, 1, 64}},
      "statement",
  };

  CHECK(error == SyntaxError{error.unexpected, "statement"});
  CHECK(error != SyntaxError{error.unexpected, "'end'"});
}
