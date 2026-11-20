#include <catch2/catch_test_macros.hpp>

#include <sstream>

#include "darthex/adapters/ConsoleReporter.hpp"
#include "darthex/domain/syntax_error.hpp"
#include "darthex/domain/token_kind.hpp"

using darthex::adapters::ConsoleReporter;
using darthex::domain::SyntaxError;
using darthex::domain::TokenKind;

TEST_CASE("reporter prints a syntax error") {
  std::ostringstream out;
  std::ostringstream err;
  ConsoleReporter reporter{out, err};
  const SyntaxError error{
      {TokenKind::kw_write, "write", {3, 7, 20}}, "'then'"};

  reporter.print_syntax_error(error);

  CHECK(out.str().empty());
  CHECK(err.str() ==
        "3:7  syntax error: expected 'then', found \"write\"\n");
}

TEST_CASE("reporter describes end of file in a syntax error") {
  std::ostringstream out;
  std::ostringstream err;
  ConsoleReporter reporter{out, err};
  const SyntaxError error{
      {TokenKind::end_of_file, "", {8, 1, 80}}, "'end'"};

  reporter.print_syntax_error(error);

  CHECK(err.str() ==
        "8:1  syntax error: expected 'end', found end of file\n");
}

TEST_CASE("reporter does not duplicate quotes around a string literal") {
  std::ostringstream out;
  std::ostringstream err;
  ConsoleReporter reporter{out, err};
  const SyntaxError error{
      {TokenKind::string_literal, "\"Area: \"", {5, 7, 42}}, "'('"};

  reporter.print_syntax_error(error);

  CHECK(err.str() ==
        "5:7  syntax error: expected '(', found \"Area: \"\n");
}

TEST_CASE("reporter prints syntax analysis success") {
  std::ostringstream out;
  std::ostringstream err;
  ConsoleReporter reporter{out, err};

  reporter.print_success();

  CHECK(out.str() == "Syntax analysis completed successfully.\n");
  CHECK(err.str().empty());
}
