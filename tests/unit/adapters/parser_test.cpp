#include <catch2/catch_test_macros.hpp>

#include <string>
#include <variant>
#include <vector>

#include "darthex/adapters/Lexer.hpp"
#include "darthex/adapters/Parser.hpp"
#include "darthex/application/ports/IParser.hpp"
#include "darthex/domain/token.hpp"
#include "darthex/domain/token_kind.hpp"

using darthex::adapters::Lexer;
using darthex::adapters::Parser;
using darthex::application::ports::ParseResult;
using darthex::domain::Token;
using darthex::domain::TokenKind;

namespace {

std::vector<Token> scan_all(const std::string& source) {
  Lexer lexer{source};
  std::vector<Token> tokens;

  while (true) {
    const auto result = lexer.next();
    REQUIRE(std::holds_alternative<Token>(result));
    const auto token = std::get<Token>(result);
    tokens.push_back(token);
    if (token.kind == TokenKind::end_of_file) {
      return tokens;
    }
  }
}

ParseResult parse(Parser& parser, const std::string& source) {
  return parser.parse(scan_all(source));
}

}  // namespace

TEST_CASE("parser accepts a basic program") {
  Parser parser;

  CHECK_FALSE(parse(
      parser,
      "program demo begin x = 1; read(x); write(\"ok\") end."));
}

TEST_CASE("parser accepts declarations and a trailing semicolon") {
  Parser parser;

  CHECK_FALSE(parse(parser,
                    "program demo "
                    "x, y : int; letter : char; value : float; "
                    "begin x = y; write(letter); end."));
}

TEST_CASE("parser accepts expressions with every arithmetic operator") {
  Parser parser;

  CHECK_FALSE(parse(parser,
                    "program expressions begin "
                    "result = 1 + 2 - 3 * 4 / 5 % 2; "
                    "logic = !ready && other || ready; "
                    "negative = -(result + 1); "
                    "write(result + 2 * 3) "
                    "end."));
}

TEST_CASE("parser accepts relational operators inside grouped expressions") {
  Parser parser;

  CHECK_FALSE(parse(parser,
                    "program comparisons begin "
                    "a = (x == y); b = (x != y); "
                    "c = (x < y); d = (x <= y); "
                    "e = (x > y); f = (x >= y) "
                    "end."));
}

TEST_CASE("parser reports the first expected item") {
  Parser parser;

  SECTION("empty input") {
    const auto error = parse(parser, "");
    REQUIRE(error);
    CHECK(error->unexpected.kind == TokenKind::end_of_file);
    CHECK(error->expected == "'program'");
  }

  SECTION("missing program name") {
    const auto error = parse(parser, "program begin x = 1 end.");
    REQUIRE(error);
    CHECK(error->unexpected.kind == TokenKind::kw_begin);
    CHECK(error->expected == "identifier");
  }

  SECTION("wrong declaration separator") {
    const auto error = parse(parser, "program demo x = int; begin x = 1 end.");
    REQUIRE(error);
    CHECK(error->unexpected.kind == TokenKind::op_assign);
    CHECK(error->expected == "':'");
  }

  SECTION("missing declaration semicolon") {
    const auto error = parse(parser, "program demo x : int begin x = 1 end.");
    REQUIRE(error);
    CHECK(error->unexpected.kind == TokenKind::kw_begin);
    CHECK(error->expected == "';'");
  }
}

TEST_CASE("parser requires at least one statement") {
  Parser parser;
  const auto error = parse(parser, "program demo begin end.");

  REQUIRE(error);
  CHECK(error->unexpected.kind == TokenKind::kw_end);
  CHECK(error->expected == "statement");
}

TEST_CASE("parser requires a semicolon between statements") {
  Parser parser;
  const auto error = parse(parser, "program demo begin x = 1 read(x) end.");

  REQUIRE(error);
  CHECK(error->unexpected.kind == TokenKind::kw_read);
  CHECK(error->expected == "';'");
}

TEST_CASE("parser validates read and write parentheses") {
  Parser parser;

  SECTION("read requires an identifier") {
    const auto error = parse(parser, "program demo begin read(1) end.");
    REQUIRE(error);
    CHECK(error->unexpected.kind == TokenKind::integer_const);
    CHECK(error->expected == "identifier");
  }

  SECTION("write requires a closing parenthesis") {
    const auto error = parse(parser, "program demo begin write(1 end.");
    REQUIRE(error);
    CHECK(error->unexpected.kind == TokenKind::kw_end);
    CHECK(error->expected == "')'");
  }
}

TEST_CASE("parser reports incomplete expressions") {
  Parser parser;

  SECTION("missing value after an operator") {
    const auto error = parse(parser, "program demo begin x = 1 + end.");
    REQUIRE(error);
    CHECK(error->unexpected.kind == TokenKind::kw_end);
    CHECK(error->expected == "expression value");
  }

  SECTION("missing closing parenthesis") {
    const auto error = parse(parser, "program demo begin x = (1 + 2 end.");
    REQUIRE(error);
    CHECK(error->unexpected.kind == TokenKind::kw_end);
    CHECK(error->expected == "')'");
  }

  SECTION("missing value after a unary operator") {
    const auto error = parse(parser, "program demo begin x = - end.");
    REQUIRE(error);
    CHECK(error->unexpected.kind == TokenKind::kw_end);
    CHECK(error->expected == "expression value");
  }
}

TEST_CASE("parser rejects an incomplete program") {
  Parser parser;

  SECTION("missing end") {
    const auto error = parse(parser, "program demo begin x = 1.");
    REQUIRE(error);
    CHECK(error->unexpected.kind == TokenKind::dot);
    CHECK(error->expected == "'end'");
  }

  SECTION("missing final dot") {
    const auto error = parse(parser, "program demo begin x = 1 end");
    REQUIRE(error);
    CHECK(error->unexpected.kind == TokenKind::end_of_file);
    CHECK(error->expected == "'.'");
  }
}

TEST_CASE("parser can be reused after an error") {
  Parser parser;

  REQUIRE(parse(parser, "program begin end."));
  CHECK_FALSE(parse(parser, "program demo begin x = 1 end."));
}
