#include <catch2/catch_test_macros.hpp>

#include "darthex/adapters/Lexer.hpp"
#include "darthex/adapters/SymbolTable.hpp"
#include "darthex/application/services/LexService.hpp"
#include "darthex/domain/token_kind.hpp"

using darthex::adapters::Lexer;
using darthex::adapters::SymbolTable;
using darthex::application::services::LexService;
using darthex::domain::TokenKind;

TEST_CASE("empty source produces only end_of_file") {
  Lexer lexer{""};
  SymbolTable symbols;
  LexService service{lexer, symbols};
  service.run();

  REQUIRE(service.tokens().size() == 1);
  CHECK(service.tokens()[0].kind == TokenKind::end_of_file);
  CHECK_FALSE(service.has_errors());
}

TEST_CASE("identifiers are installed in the symbol table") {
  Lexer lexer{"x y x"};
  SymbolTable symbols;
  LexService service{lexer, symbols};
  service.run();

  CHECK_FALSE(service.has_errors());
  REQUIRE(symbols.find("x").has_value());
  REQUIRE(symbols.find("y").has_value());
  CHECK(symbols.size() == 2);
}

TEST_CASE("repeated identifier installs only once") {
  Lexer lexer{"count count count"};
  SymbolTable symbols;
  LexService service{lexer, symbols};
  service.run();

  CHECK(symbols.size() == 1);
  REQUIRE(symbols.find("count").has_value());
}

TEST_CASE("keywords are installed in the symbol table") {
  Lexer lexer{"if then else"};
  SymbolTable symbols;
  LexService service{lexer, symbols};
  service.run();

  CHECK(symbols.size() == 3);
  REQUIRE(symbols.find("if").has_value());
  REQUIRE(symbols.find("then").has_value());
  REQUIRE(symbols.find("else").has_value());
}

TEST_CASE("identifiers and keywords are both installed") {
  Lexer lexer{"x read y write"};
  SymbolTable symbols;
  LexService service{lexer, symbols};
  service.run();

  CHECK(symbols.size() == 4);
  REQUIRE(symbols.find("x").has_value());
  REQUIRE(symbols.find("y").has_value());
  REQUIRE(symbols.find("read").has_value());
  REQUIRE(symbols.find("write").has_value());
}

TEST_CASE("tokens are collected in order") {
  // `:` and `=` are two distinct tokens; there is no `:=` compound operator
  Lexer lexer{"x := 42"};
  SymbolTable symbols;
  LexService service{lexer, symbols};
  service.run();

  REQUIRE(service.tokens().size() == 5);
  CHECK(service.tokens()[0].kind == TokenKind::identifier);
  CHECK(service.tokens()[1].kind == TokenKind::colon);
  CHECK(service.tokens()[2].kind == TokenKind::op_assign);
  CHECK(service.tokens()[3].kind == TokenKind::integer_const);
  CHECK(service.tokens()[4].kind == TokenKind::end_of_file);
}

TEST_CASE("lexical errors are collected and tokens continue") {
  Lexer lexer{"x @@ y"};
  SymbolTable symbols;
  LexService service{lexer, symbols};
  service.run();

  CHECK(service.has_errors());
  CHECK(service.errors().size() == 2);

  REQUIRE(service.tokens().size() >= 2);
  CHECK(service.tokens()[0].kind == TokenKind::identifier);
  CHECK(service.tokens()[0].lexeme == "x");
}

TEST_CASE("mixed valid and invalid source collects both correctly") {
  Lexer lexer{"a := 1.5"};
  SymbolTable symbols;
  LexService service{lexer, symbols};
  service.run();

  CHECK_FALSE(service.has_errors());
  REQUIRE(service.tokens().size() == 5);
  CHECK(service.tokens()[3].kind == TokenKind::float_const);
  CHECK(service.tokens()[3].lexeme == "1.5");

  REQUIRE(symbols.find("a").has_value());
}

TEST_CASE("run on already-exhausted lexer produces only end_of_file") {
  Lexer lexer{"z"};
  SymbolTable symbols;
  LexService service{lexer, symbols};
  service.run();
  CHECK(service.tokens().size() == 2);  // identifier + eof
}
